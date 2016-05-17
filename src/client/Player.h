#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include "../Graphics/Objects/Entity.h"

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

class Player : public Entity
{
    // Player is made up of a model with a camera following it
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Model> model;

    // How up/down camera is
    float camAngle;

    // Path name for chicken model texture
    std::string modelFile; 

public:

    Player();
    Player(int client_id);
    ~Player();

    // Inherited via Entity
    virtual void Update() override;
    virtual void Spawn(float x, float y, float z) override;
    virtual void Draw() const override;

    void SetModelFile(std::string fileName);

    // Process movement
    void ProcessKeyboard(DIRECTION direction, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);

    glm::vec3 CameraPosition() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat3 GetNormalMatrix() const;
    
    glm::mat4 GetToWorld() { return toWorld; };
    void SetToWorld(glm::mat4 newToWorld) { toWorld = newToWorld;  };
    
    float GetCamAngle() { return camAngle; };
    void SetCamAngle(float newAngle) { camAngle = newAngle; };

	int GetID() { return id; };

	glm::quat GetOrientation() { return Orientation();  }

private:
    int id;
	int tick = 0;
};