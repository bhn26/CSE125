#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "client/Window.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
    friend class Player;
private:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

    GLfloat zNear, zFar;

public:
    // Constructor 
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f));

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() { return glm::lookAt(this->position, this->position + this->front, this->up); }
    glm::mat4 Camera::GetPerspectiveMatrix() { return glm::perspective(this->zoom, ((GLfloat)Window::width) / Window::height, zNear, zFar); }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);

    const glm::vec3& Position() const { return this->position; }
    const glm::vec3& Up() const { return this->up; }
    const glm::vec3& Front() const { return front; }
    const glm::vec3& Right() const { return right; }
    const glm::vec3& WorldUp() const { return worldUp; }
    GLfloat Zoom() const { return this->zoom; }
    GLfloat Speed() const { return movementSpeed; }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void UpdateCameraVectors();

    // Default camera values
    static const GLfloat SPEED;
    static const GLfloat SENSITIVTY;
    static const GLfloat ZOOM;
    static const GLfloat ZNEAR;
    static const GLfloat ZFAR;
};
