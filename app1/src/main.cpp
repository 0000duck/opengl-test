// Local Headers
#include "main.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>




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
        glm::mat4 mvp = glm::mat4(1.0f);
        //mvp = glm::translate(mvp, glm::vec3(0.5f, -0.5f, 0.0f));
        mvp = glm::rotate(mvp, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
        prog.loadUniform("mvp", mvp);
        glBindVertexArray(vao.id);
        glUseProgram(prog.id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
