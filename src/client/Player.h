#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include "../Graphics/Objects/Entity.h"
#include "../Graphics/Animation/AnimationPlayer.h"

class Camera;
class Model;
class Shader;

namespace Animation
{
    class AnimatedModel;
}

enum DIRECTION
{
    D_FORWARD,
    D_BACKWARD,
    D_LEFT,
    D_RIGHT,
    D_UP,
    D_DOWN
};


class Player : public Entity, public Animation::AnimationPlayer::Listener
{

public:
    enum STATE
    {
        IDLE,
        JUMP,
        WALK,
        DANCE,
        PECK,
    };

    Player(float x = 0.0f, float y = 0.0f, float z = 0.0f,
		float rotW = 0.0f, float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f);
    Player(int client_id);
    ~Player();

    // Inherited via Entity
    virtual void Update() override;
    virtual void Spawn(float x, float y, float z) override;
    virtual void Draw() const override;

    virtual void MoveTo(float x, float y, float z) override;

    void SetModelFile(std::string fileName);

    // Process movement
    void ProcessKeyboard(DIRECTION direction, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void ProcessViewMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);

    glm::vec3 CameraPosition() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetPerspectiveMatrix() const;
    glm::mat3 GetNormalMatrix() const;

	int GetID() { return id; };
	int GetClassId() { return class_id; }

	glm::quat GetOrientation() { return Orientation();  }

    void ChangeState(STATE state);                  // Will change model state and player state
    void SetState(STATE state) { m_state = state; }     // Simply Sets the state without changing the model

private:
    // Player is made up of a model with a camera following it
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Model> model;
    //std::unique_ptr<Animation::AnimatedModel> m_model;

    std::unordered_map<std::string, std::string> m_animNames;

    // How up/down camera is
    float camAngle;
    float m_HViewSensitivity = 7.5f;
    float m_VViewSensitivity = 5.0f;
    int id;
    int tick = 0;

    // Path name for chicken model texture
    std::string modelFile;

    STATE m_state;
    float m_lastTime_t;     // Test
    glm::vec3 m_lastPos_t;

    // Inherited via Listener
    virtual void OnFinish() override;

};
