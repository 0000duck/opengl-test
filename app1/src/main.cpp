#include "main.hpp"
#include "camera.hpp"
#include "lights.hpp"

#include <GLFW/glfw3.h>


double lastTime;
double deltaTime;

unsigned mWidth = DEFAULT_WIDTH;
unsigned mHeight = DEFAULT_HEIGHT;

double lastX;
double lastY;
bool cameraControl = false;
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));

glm::mat4 model(1.0f);
glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);


glm::mat4 setupMvp() {
    glm::mat4 mvp(1.0f);
    mvp *= projection;
    mvp *= view;
    mvp *= model;
    return mvp;

}

void framebufferSizeCallback(GLFWwindow *, int width, int height) {
    glViewport(0, 0, width, height);
    mWidth = unsigned(width);
    mHeight = unsigned(height);
    projection = glm::perspective(glm::radians(45.0f), (float) mWidth / (float) mHeight, 0.1f, 200.0f);
}

void mouseCallback(GLFWwindow *, double xpos, double ypos) {
    if (!cameraControl)
        return;
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    camera.processMouseMovement(float(xoffset), float(yoffset));
    lastX = xpos;
    lastY = ypos;
}

void mouseButtonFunc(GLFWwindow *win, int button, int action, int) {
    glfwGetCursorPos(win, &lastX, &lastY);
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        switch (action) {
            case GLFW_PRESS:
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cameraControl = true;
                break;
            case GLFW_RELEASE:
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cameraControl = false;
                break;
            default:
                break;
        }
    }
}

void processInput(GLFWwindow *window) {
    auto dt = float(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, dt);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.processKeyboard(DOWN, dt);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(UP, dt);
}


int main(int, char **) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);
    glfwSetWindowAspectRatio(mWindow, mWidth, mHeight);

    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glfwSetCursorPosCallback(mWindow, mouseCallback);
    glfwSetMouseButtonCallback(mWindow, mouseButtonFunc);
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
    camera.movementSpeed = 20.0f;
    projection = glm::perspective(glm::radians(45.0f), (float) mWidth / (float) mHeight, 0.1f, 200.0f);

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);

    Mesh mesh(PROJECT_SOURCE_DIR "/models/Cup.3DS");
    Lights lights(Mesh(PROJECT_SOURCE_DIR "/models/Sphere.3ds"), 0.05f);
    lights.pointLights.emplace_back(glm::vec3(2.0f, 2.0f, 8.0f));

    ShaderProgram shaderProgram;
    {
        Shader<GL_VERTEX_SHADER> vert(SHADERS_DIR "a1.vert");
        Shader<GL_FRAGMENT_SHADER> frag(SHADERS_DIR "a1.frag");
        shaderProgram.linkShaderProgram(vert, frag);
    }

    lastTime = glfwGetTime();
    while (glfwWindowShouldClose(mWindow) == 0) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(mWindow);

        glClearColor(0.7f, 0.75f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::rotate(glm::mat4(1.0f), (float) glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
        //model = glm::rotate_slow(model, (float) glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view = camera.getViewMatrix();

        glm::mat4 mvp = setupMvp();
        glm::mat3 normalm(glm::transpose(glm::inverse(model)));

        lights.loadPointIntoUniform(shaderProgram);
        shaderProgram.loadUniform("viewerPos", camera.getPosition());
        mesh.draw(shaderProgram, mvp, model, normalm);

        lights.drawPointLights(shaderProgram, view, projection);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}
