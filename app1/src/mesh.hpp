#ifndef APP1_MESH_HPP
#define APP1_MESH_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <glad/glad.h>
#include <assimp/mesh.h>
#include "types.hpp"


class Mesh {
private:
    static std::unique_ptr<ShaderProgram> shaderProgram;

    struct MeshElement {
        std::unique_ptr<BufferObject<GL_ELEMENT_ARRAY_BUFFER, GLuint> > elements;
        std::unique_ptr<BufferObject<GL_ARRAY_BUFFER> > vertices;
        std::unique_ptr<BufferObject<GL_ARRAY_BUFFER> > normals;
        GLuint vao;
        size_t numElements;

        MeshElement(MeshElement const &) = delete;

        MeshElement &operator=(MeshElement const &) = delete;

        MeshElement(MeshElement &&other) noexcept {
            vao = other.vao;
            elements = std::move(other.elements);
            vertices = std::move(other.vertices);
            normals = std::move(other.normals);
        }

        explicit MeshElement(const aiMesh *);

        ~MeshElement();

        std::unique_ptr<BufferObject<GL_ELEMENT_ARRAY_BUFFER, GLuint> > loadElements(const aiMesh *);

        std::unique_ptr<BufferObject<GL_ARRAY_BUFFER>> loadVertices(const aiMesh *);

        std::unique_ptr<BufferObject<GL_ARRAY_BUFFER>> loadNormals(const aiMesh *);

        void bind();

        void draw(glm::mat4& mvp, glm::mat4& model, glm::mat3& normalm);
    };

    std::vector<MeshElement> meshElements;
public:
    Mesh(Mesh const &) = delete;

    Mesh &operator=(Mesh const &) = delete;

    Mesh(Mesh &&other) {
        meshElements = std::move(other.meshElements);
    }

    explicit Mesh(std::string);

    ~Mesh();

    void draw(glm::mat4& mvp, glm::mat4& model, glm::mat3& normalm) {
        for (auto &m: meshElements) m.draw(mvp, model, normalm);
    };
};


#endif //APP1_MESH_HPP
