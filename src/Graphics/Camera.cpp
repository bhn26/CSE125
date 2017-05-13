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
    : m_position(position)
    , m_worldUp(up)
    , m_front(front)
    , m_movementSpeed(SPEED)
    , m_mouseSensitivity(SENSITIVTY)
    , m_zoom(ZOOM)
    , m_zNear(ZNEAR)
    , m_zFar(ZFAR)
{
    UpdateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement direction, GLfloat deltaTime)
{
    GLfloat velocity = m_movementSpeed * deltaTime;
    if (direction == CameraMovement::Forward)
        m_position += m_front * velocity;
    if (direction == CameraMovement::Backward)
        m_position -= m_front * velocity;
    if (direction == CameraMovement::Left)
        m_position -= m_right * velocity;
    if (direction == CameraMovement::Right)
        m_position += m_right * velocity;
    if (direction == CameraMovement::Up)
        m_position += m_up * velocity;
    if (direction == CameraMovement::Down)
        m_position -= m_up * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    m_front =
        glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), m_worldUp)) * m_front;

    // Update Front, Right and Up Vectors using the updated Eular angles
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (m_zoom <= 1.0f)
    {
        m_zoom = 1.0f;
    }
    else if (m_zoom >= 45.0f)
    {
        m_zoom = 45.0f;
    }
    else
    {
        m_zoom -= yoffset;
    }
}

void Camera::UpdateCameraVectors()
{
    m_front = glm::normalize(m_front);
    // Normalize the vectors, because their length gets closer to 0 the more you look up or down
    // which results in slower movement.
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
