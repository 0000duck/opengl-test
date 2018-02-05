#ifndef APP1_TYPES_HPP
#define APP1_TYPES_HPP

#include <stdexcept>
#include <string>
#include <fstream>
#include <locale>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


template<GLenum bufferType, typename DataType = GLfloat>
class BufferObject {
public:
    BufferObject(BufferObject const &) = delete;

    BufferObject &operator=(BufferObject const &) = delete;

    GLuint id;

    BufferObject() {
        glGenBuffers(1, &id);
    }

    ~BufferObject() {
        glDeleteBuffers(1, &id);
    }

    void loadData(size_t size, DataType data[]) {
        bind();
        glBufferData(bufferType, size * sizeof(DataType), data, GL_STATIC_DRAW);
    }

    void bind() {
        glBindBuffer(bufferType, id);
    }

    void unbind() {
        glBindBuffer(bufferType, 0);
    }
};

template<GLenum shaderType>
class Shader {
private:
    std::string readFile(const std::string &filePath) {
        std::string content;
        std::ifstream fileStream(filePath, std::ios::in);

        if (!fileStream.is_open()) {
            std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
            return "";
        }

        std::string line = "";
        while (!fileStream.eof()) {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }

        fileStream.close();
        return content;
    }

public:
    GLuint id;

    Shader(Shader const &) = delete;

    Shader &operator=(Shader const &) = delete;

    explicit Shader(const std::string &filename) {
        id = glCreateShader(shaderType);
        std::string src1 = readFile(filename);
        const char *src = src1.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);
        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            throw std::runtime_error(infoLog);
        }
    }

    ~Shader() {
        glDeleteShader(id);
    }
};

class ShaderProgram {
private:

    void doLoad(GLint loc, const glm::vec4 &v) const {
        glUniform4f(loc, v.x, v.y, v.z, v.w);
    }

    void doLoad(GLint loc, const glm::mat4 &m) const {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    }

    void doLoad(GLint loc, const glm::mat3 &m) const {
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    }

//    void doLoad(GLint loc, bool b) const {
//        glUniform1i(loc, b ? 1 : 0);
//    }

    void doLoad(GLint loc, const glm::vec3 &v) const {
        glUniform3f(loc, v.x, v.y, v.z);
    }

public:
    GLuint id;

    ShaderProgram(ShaderProgram const &) = delete;

    ShaderProgram &operator=(ShaderProgram const &) = delete;

    ShaderProgram() {
        id = glCreateProgram();
    }

    template<GLenum first>
    void linkShaderProgram(const Shader<first> &s) {
        glAttachShader(id, s.id);
        glLinkProgram(id);

        int success;
        const size_t RUNTIME_ERROR_MSG_SIZE = 512;
        char infoLog[RUNTIME_ERROR_MSG_SIZE];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, RUNTIME_ERROR_MSG_SIZE, NULL, infoLog);
            throw std::runtime_error(infoLog);
        }
    }

    template<GLenum first, typename... Args>
    void linkShaderProgram(const Shader<first> &s, const Args &... rest) {
        glAttachShader(id, s.id);
        linkShaderProgram(rest...);
    }

    template<typename Arg>
    void loadUniform(const std::string &varname, const Arg &input) const {
        GLint location = glGetUniformLocation(id, varname.c_str());
        glUseProgram(id);
        doLoad(location, input);
    }

    void loadUniformInt(const std::string& varname, int input) const {
        GLint location = glGetUniformLocation(id, varname.c_str());
        glUseProgram(id);
        glUniform1i(location, input);
    }

    void loadUniformFloat(const std::string& varname, float input) const {
        GLint location = glGetUniformLocation(id, varname.c_str());
        glUseProgram(id);
        glUniform1f(location, input);
    }

    void use() const {
        glUseProgram(id);
    }

};

class Texture {
public:
    GLuint id;

    Texture (Texture const &) = delete;

    Texture  &operator=(Texture  const &) = delete;

    Texture (Texture&& other) {
        id = other.id;
    }

    Texture (GLubyte* data, int width, int height) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    ~Texture() {
        glDeleteTextures(1, &id);
    }

    void bind() {
        glBindTexture(GL_TEXTURE_2D, id);
    }
};

#endif //APP1_TYPES_HPP
