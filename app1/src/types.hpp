#ifndef APP1_TYPES_HPP
#define APP1_TYPES_HPP

class VBO {
private:
    GLuint id;
public:
    VBO() {
        glGenBuffers(1, &id);
    }
    ~VBO() {
        glDeleteBuffers(1, &id);
    }
    void loadData (size_t size, GLfloat* data) {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
};

#endif //APP1_TYPES_HPP
