// modified from https://learnopengl.com/#!Getting-started/Camera

#ifndef APP1_CAMERA_HPP
#define APP1_CAMERA_HPP


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


enum CameraMovementDir {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 15.0f;
const float SENSITIVITY = 0.2f;
const float ZOOM = 45.0f;

class Camera {
    glm::vec3 posV;
    glm::vec3 frontV;
    glm::vec3 upV;
    glm::vec3 rightV;
    glm::vec3 worldUpV;
    float xi;
    float eta;

public:
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH) : frontV(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
                                                   mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        posV = position;
        worldUpV = up;
        xi = yaw;
        eta = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(posV, posV + frontV, upV);
    }

    void processKeyboard(CameraMovementDir direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        switch (direction) {
            case FORWARD:
                posV += frontV * velocity;
                break;
            case BACKWARD:
                posV -= frontV * velocity;
                break;
            case LEFT:
                posV -= rightV * velocity;
                break;
            case RIGHT:
                posV += rightV * velocity;
                break;
            case UP:
                posV += worldUpV * velocity;
                break;
            case DOWN:
                posV -= worldUpV * velocity;
                break;
        }
    }

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        xi += xoffset;
        eta += yoffset;

        if (constrainPitch) {
            if (eta > 89.5f)
                eta = 89.5f;
            if (eta < -89.5f)
                eta = -89.5f;
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset) {
        if (zoom >= 1.0f && zoom <= 45.0f)
            zoom -= yoffset;
        if (zoom <= 1.0f)
            zoom = 1.0f;
        if (zoom >= 45.0f)
            zoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        glm::vec3 front(0.0f);
        front.x = glm::cos(glm::radians(xi)) * glm::cos(glm::radians(eta));
        front.y = glm::sin(glm::radians(eta));
        front.z = glm::sin(glm::radians(xi)) * glm::cos(glm::radians(eta));
        frontV = glm::normalize(front);
        rightV = glm::normalize(glm::cross(frontV, worldUpV));
        upV = glm::normalize(glm::cross(rightV, frontV));
    }
};


#endif //APP1_CAMERA_HPP
