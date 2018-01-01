#ifndef APP1_MESH_HPP
#define APP1_MESH_HPP

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <assimp/mesh.h>

#include "types.hpp"


class Mesh {
private:
    struct MeshElement {
        std::unique_ptr<BufferObject<GL_ELEMENT_ARRAY_BUFFER, GLuint> > elements;
        std::unique_ptr<BufferObject<GL_ARRAY_BUFFER> > vertices;
        std::unique_ptr<BufferObject<GL_ARRAY_BUFFER> > normals;
        GLuint vao;
        size_t numElements;
        glm::vec3 color;

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

        void bind() const;

        void draw(const ShaderProgram &prog,
                  const glm::mat4 &mvp,
                  const glm::mat4 &model,
                  const glm::mat3 &normalm) const;

        void drawUnlit(const ShaderProgram &prog,
                       const glm::mat4 &mvp,
                       glm::vec3 color) const;
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

    void draw(const ShaderProgram &prog,
              const glm::mat4 &mvp,
              const glm::mat4 &model,
              const glm::mat3 &normalm) const {
        for (auto &m: meshElements) m.draw(prog, mvp, model, normalm);
    };

    void drawUnlit(const ShaderProgram &prog,
                   const glm::mat4 &mvp,
                   glm::vec3 color) const {
        for (auto &m: meshElements) m.drawUnlit(prog, mvp, color);
    }

    glm::vec3 color = glm::vec3(0.5f);
};


#endif //APP1_MESH_HPP
