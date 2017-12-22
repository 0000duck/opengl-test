#ifndef APP1_TYPES_HPP
#define APP1_TYPES_HPP
#include <stdexcept>
#include <string>
#include <fstream>
#include <locale>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


std::string readFile(const std::string& filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

template<GLenum bufferType, typename DataType = GLfloat>
class BufferObject {
public:
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

//template<typename VertexType>


template <GLenum shaderType>
class Shader {
public:
	GLuint id;
    explicit Shader(const std::string& filename) {
		id = glCreateShader(shaderType);
        std::string src1 = readFile(filename);
        const char* src = src1.c_str();
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
//    template<typename Arg>
//    void doLoad(GLuint, const Arg&) {
//    }

    void doLoad(GLint loc, const glm::vec4& v) const {
        glUniform4f(loc, v.x, v.y, v.z, v.w);
    }
    void doLoad(GLint loc, const glm::mat4& m) const {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    }
public:
    GLuint id;

    ShaderProgram() {
        id = glCreateProgram();
    }
    template <GLenum first>
    void linkShaderProgram(const Shader<first>& s) {
        glAttachShader(id, s.id);
        glLinkProgram(id);

        int success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            throw std::runtime_error(infoLog);
        }
    }
    template <GLenum first, typename... Args>
    void linkShaderProgram(const Shader<first>& s, Args... rest) {
        glAttachShader(id, s.id);
        linkShaderProgram(rest...);
    }

    template<typename Arg>
    void loadUniform(std::string varname, const Arg& input) const {
        GLint location = glGetUniformLocation(id, varname.c_str());
        glUseProgram(id);
        doLoad(location, input);
    }

};

#endif //APP1_TYPES_HPP
