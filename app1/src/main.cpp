// Local Headers
#include "main.hpp"

// System Headers
#include <GLFW/glfw3.h>

// Standard Headers


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

int main(int, char **) {
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

    std::vector<Vertex> vertices{
            Vertex(-1.0f, -1.0f, -1.0f),
            Vertex(-1.0f, -1.0f, 1.0f),
            Vertex(-1.0f, 1.0f, 1.0f),
            Vertex(1.0f, 1.0f, 1.0f)
    };
    GLuint indices[] = {  // note that we start from 0!
            0, 1, 2,
            0, 2, 3,
            1, 2, 3,
            1, 0, 3
    };
    ShaderProgram prog;
    {
        Shader<GL_VERTEX_SHADER> vert("src/shaders/a1.vert");
        Shader<GL_FRAGMENT_SHADER> frag("src/shaders/a1.frag");
        prog.linkShaderProgram(vert, frag);
    }
    VAO vao;
    vao.loadVertices(vertices);
    vao.loadIndices(sizeof(indices) / sizeof(GLuint), indices);
    projection = glm::perspective(glm::radians(45.0f), (float) mWidth / (float) mHeight, 0.01f, 100.1f);

    glEnable(GL_DEPTH_TEST);

    while (glfwWindowShouldClose(mWindow) == 0) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        glClearColor(0.7f, 0.75f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::rotate_slow(glm::mat4(1.0f), (float) glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::rotate_slow(model, (float) glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));
        view = glm::rotate_slow(view, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        setupMvp(prog);
        glBindVertexArray(vao.id);
        glUseProgram(prog.id);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
