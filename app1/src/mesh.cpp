#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "mesh.hpp"

std::unique_ptr<ShaderProgram> Mesh::shaderProgram;

Mesh::Mesh(std::string filename) {
    if (!shaderProgram) {
        shaderProgram = std::make_unique<ShaderProgram>();
        Shader<GL_VERTEX_SHADER> vert(SHADERS_DIR "a1.vert");
        Shader<GL_FRAGMENT_SHADER> frag(SHADERS_DIR "a1.frag");
        shaderProgram->linkShaderProgram(vert, frag);
    }
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename.c_str(), 0);
    if (scene == nullptr)
        throw std::runtime_error(importer.GetErrorString());
    if (!scene->HasMeshes())
        throw std::runtime_error("No meshes in file");
    for (unsigned i = 0; i < scene->mNumMeshes; i++) {
        meshElements.emplace_back(scene->mMeshes[i]);
    }
}

Mesh::~Mesh() {
    meshElements.clear();
}

Mesh::MeshElement::MeshElement(const aiMesh *mesh) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    if (!mesh->HasFaces())
        throw std::runtime_error("Mesh has no faces");
    if (!mesh->HasPositions())
        throw std::runtime_error("Mesh has no vertices");
    elements = loadElements(mesh);
    vertices = loadVertices(mesh);
    if (mesh->HasNormals())
        normals = loadNormals(mesh);
}

Mesh::MeshElement::~MeshElement() {
//    elements.reset();
//    vertices.reset();
//    normals.reset();
    glDeleteVertexArrays(1, &vao);
}

std::unique_ptr<BufferObject<GL_ELEMENT_ARRAY_BUFFER, GLuint> > Mesh::MeshElement::loadElements(
        const aiMesh *mesh) {
    auto buffer = std::make_unique<BufferObject<GL_ELEMENT_ARRAY_BUFFER, GLuint>>();
    std::vector<GLuint> indices(mesh->mNumFaces * 3);
    for (unsigned i = 0; i < mesh->mNumFaces; i++) {
        if (mesh->mFaces[i].mNumIndices > 3)
            throw std::runtime_error("Too many vertices in a primitive");
        for (unsigned j = 0; j < 3; j++)
            indices[i * 3 + j] = mesh->mFaces[i].mIndices[j];
    }
    bind();
    buffer->loadData(indices.size(), indices.data());
    numElements = indices.size();
    return buffer;
}

std::unique_ptr<BufferObject<GL_ARRAY_BUFFER>> Mesh::MeshElement::loadVertices(
        const aiMesh *mesh) {
    auto buffer = std::make_unique<BufferObject<GL_ARRAY_BUFFER>>();
    std::vector<GLfloat> vertices(mesh->mNumVertices * 3);
    for (unsigned i = 0; i < mesh->mNumVertices; i++) {
        vertices[i * 3] = mesh->mVertices[i].x;
        vertices[i * 3 + 1] = mesh->mVertices[i].y;
        vertices[i * 3 + 2] = mesh->mVertices[i].z;
    }
    bind();
    buffer->loadData(vertices.size(), vertices.data());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    return buffer;
}

std::unique_ptr<BufferObject<GL_ARRAY_BUFFER>> Mesh::MeshElement::loadNormals(
        const aiMesh *mesh) {
    auto buffer = std::make_unique<BufferObject<GL_ARRAY_BUFFER>>();
    std::vector<GLfloat> vertices(mesh->mNumVertices * 3);
    for (unsigned i = 0; i < mesh->mNumVertices; i++) {
        vertices[i * 3] = mesh->mNormals[i].x;
        vertices[i * 3 + 1] = mesh->mNormals[i].y;
        vertices[i * 3 + 2] = mesh->mNormals[i].z;
    }
    bind();
    buffer->loadData(vertices.size(), vertices.data());
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    return buffer;
}

void Mesh::MeshElement::bind() {
    glBindVertexArray(vao);
}

void Mesh::MeshElement::draw(glm::mat4 &mvp) {
    bind();
    glUseProgram(shaderProgram->id);
    shaderProgram->loadUniform("mvp", mvp);
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, nullptr);
}
