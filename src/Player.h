#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera;
class Model;
class Shader;

enum DIRECTION
{
    D_FORWARD,
    D_BACKWARD,
    D_LEFT,
    D_RIGHT,
    D_UP,
    D_DOWN
};

class Player
{
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Model> model;

    glm::mat4 toWorld;
    glm::mat4 cameraTransOffset;

    float camAngle;

public:
    std::shared_ptr<Shader> shader;

    Player();
    ~Player();

    void Draw() const;

    // Process movement
    void ProcessKeyboard(DIRECTION direction, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);

    glm::vec3 CameraPosition() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat3 GetNormalMatrix() const;
};