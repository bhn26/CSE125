#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../client/ClientGame.h"

// Default camera values
const GLfloat Camera::SPEED = 0.5f;
const GLfloat Camera::SENSITIVTY = 0.25f;
const GLfloat Camera::ZOOM = 45.0f;
const GLfloat Camera::ZNEAR = 0.1f;
const GLfloat Camera::ZFAR = 1000.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front)
    : position(position), worldUp(up), front(front),
    movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM), zNear(ZNEAR), zFar(ZFAR)
{
    this->UpdateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->movementSpeed * deltaTime;
    if (direction == CameraMovement::Forward)
        this->position += this->front * velocity;
    if (direction == CameraMovement::Backward)
        this->position -= this->front * velocity;
    if (direction == CameraMovement::Left)
        this->position -= this->right * velocity;
    if (direction == CameraMovement::Right)
        this->position += this->right * velocity;
    if (direction == CameraMovement::Up)
        this->position += this->up * velocity;
    if (direction == CameraMovement::Down)
        this->position -= this->up * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    this->front = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), this->worldUp)) * this->front;

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->zoom >= 1.0f && this->zoom <= 45.0f)
        this->zoom -= yoffset;
    if (this->zoom <= 1.0f)
        this->zoom = 1.0f;
    if (this->zoom >= 45.0f)
        this->zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
