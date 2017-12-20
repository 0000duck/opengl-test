#ifndef APP1_TYPES_HPP
#define APP1_TYPES_HPP
#include <stdexcept>
#include <string>
#include <fstream>
#include <locale>
#include <iostream>

namespace fs= std::filesystem;

class VBO {
public:
    GLuint id;
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

template <GLenum shaderType>
class Shader {
public:
	GLuint id;
    explicit Shader(const std::string& filename) {
		id = glCreateShader(shaderType);
        std::ifstream fd(filename);
        const char* src = std::string(std::istreambuf_iterator<char>(fd),
                               (std::istreambuf_iterator<char>())).c_str();
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
};

#endif //APP1_TYPES_HPP
