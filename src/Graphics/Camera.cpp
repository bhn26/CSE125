#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../client/ClientGame.h"

// Default camera values
const GLfloat Camera::YAW = -90.0f;
const GLfloat Camera::PITCH = 0.0f;
const GLfloat Camera::SPEED = 0.5f;
const GLfloat Camera::SENSITIVTY = 0.25f;
const GLfloat Camera::ZOOM = 45.0f;
const GLfloat Camera::ZNEAR = 0.1f;
const GLfloat Camera::ZFAR = 1000.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : position(position), worldUp(up), front(glm::vec3(0.0f, 0.0f, -1.0f)), yaw(yaw), pitch(pitch), 
        movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM), zNear(ZNEAR), zFar(ZFAR)

{
    this->UpdateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
    : Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch)
{
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->movementSpeed * deltaTime;
    if (direction == FORWARD)
        this->position += this->front * velocity;
    if (direction == BACKWARD)
        this->position -= this->front * velocity;
    if (direction == LEFT)
        this->position -= this->right * velocity;
    if (direction == RIGHT)
        this->position += this->right * velocity;
    if (direction == UP)
        this->position += this->up * velocity;
    if (direction == DOWN)
        this->position -= this->up * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->pitch > 89.0f)
            this->pitch = 89.0f;
        if (this->pitch < -89.0f)
            this->pitch = -89.0f;
    }

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
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
