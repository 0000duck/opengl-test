// Local Headers
#include "main.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

glm::mat4 model(1.0f);
glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);

void setupMvp(const ShaderProgram& prog) {
    glm::mat4 mvp(1.0f);
    mvp*= projection;
    mvp*= view;
    mvp*= model;
    prog.loadUniform("mvp", mvp);
}

int main(int argc, char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));


    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    ShaderProgram prog;
    {
        Shader<GL_VERTEX_SHADER> vert("src/shaders/a1.vert");
        Shader<GL_FRAGMENT_SHADER> frag("src/shaders/a1.frag");
        prog.linkShaderProgram(vert, frag);
    }
    VAO vao;
    vao.loadVertices(sizeof(vertices), vertices);
    vao.loadIndices(sizeof(indices), indices);
    projection = glm::perspective(glm::radians(45.0f), (float)mWidth/(float)mHeight, 0.0f, 100.1f);

    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        glClearColor(0.7f, 0.75f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        GLfloat blueValue = (sin(timeValue+1.0) / 2.0f) + 0.5f;
        GLfloat redValue = (sin(timeValue+2.0) / 2.0f) + 0.5f;
        prog.loadUniform("myColor", glm::vec4(redValue, greenValue, blueValue, 1.0f));
        model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::rotate(view, 0.5f, glm::vec3(1.0f,1.0f,1.0f));
        setupMvp(prog);
        glBindVertexArray(vao.id);
        glUseProgram(prog.id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
