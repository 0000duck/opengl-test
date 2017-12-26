#ifndef APP1_VAO_HPP
#define APP1_VAO_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "types.hpp"

struct Vertex {
    glm::vec3 coords;
    glm::vec3 normal;

    Vertex(GLfloat x, GLfloat y, GLfloat z, glm::vec3 _normal = glm::vec3(0.0f)) {
        coords = glm::vec3(x, y, z);
        normal = _normal;
    }
};

class VAO {
private:
    BufferObject<GL_ARRAY_BUFFER> vbo;
    BufferObject<GL_ARRAY_BUFFER> normals;
    BufferObject<GL_ELEMENT_ARRAY_BUFFER, GLuint> ebo;

public:
    GLuint id;

    VAO(VAO const &) = delete;
    VAO & operator=(VAO const &) = delete;
    VAO() {
        glGenVertexArrays(1, &id);
    }

    ~VAO() {
        glDeleteVertexArrays(1, &id);
    }

    void loadVertices(size_t size, GLfloat data[]) {
        bind();
        vbo.loadData(size, data);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        unbind();
        vbo.unbind();
    }

    void loadVertices(const std::vector<Vertex> &vertices) {
        bind();
        std::vector<GLfloat> buffer;

        buffer.reserve(vertices.size() * glm::vec3::length());
        for (const Vertex &v: vertices) {
            buffer.push_back(v.coords.x);
            buffer.push_back(v.coords.y);
            buffer.push_back(v.coords.z);
        }
        vbo.loadData(buffer.size(), buffer.data());
        buffer.clear();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        vbo.unbind();

        buffer.reserve(vertices.size() * glm::vec3::length());
        for (const Vertex &v: vertices) {
            buffer.push_back(v.normal.x);
            buffer.push_back(v.normal.y);
            buffer.push_back(v.normal.z);
        }
        normals.loadData(buffer.size(), buffer.data());
        buffer.clear();
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(1);
        normals.unbind();

        unbind();
    }

    void loadIndices(size_t size, GLuint data[]) {
        bind();
        ebo.loadData(size, data);
        unbind();
    };

    void bind() {
        glBindVertexArray(id);
    }

    void unbind() {
        glBindVertexArray(0);
        ebo.unbind();
    }
};

#endif //APP1_VAO_HPP
