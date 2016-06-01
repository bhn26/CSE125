#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include "Graphics/Objects/Entity.h"
#include "Graphics/Animation/AnimationPlayer.h"

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
    enum State
    {
        IDLE,
        JUMP,
        WALK,
        DANCE,
        ATTACK,
        DEATH,
        TAUNT,
        PECK,
    };

    Player(float x = 0.0f, float y = 0.0f, float z = 0.0f,
           float rotW = 0.0f, float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f);
    Player(int client_id);
    ~Player();

    // Inherited via Entity
    virtual void Update(float deltaTime) override;
    virtual void Draw() const override;
    virtual void SetShaderUniforms() const override;

    virtual void MoveTo(float x, float y, float z) override;
    virtual void RotateTo(const glm::quat& newOrientation) override;
    void Jump() { ChangeState(State::JUMP); }
    void Dance() { ChangeState(State::DANCE); }
    void Attack() { ChangeState(State::ATTACK); }
	void Die() { ChangeState(State::DEATH); alive = false; }
    void Peck() { ChangeState(State::PECK); }


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

    int GetID() const { return id; };
    int GetClassId() const { return class_id; }

    void ChangeState(State state);                  // Will change model state and player state
    void SetState(State state) { m_state = state; }     // Simply Sets the state without changing the model

    int GetScore() const override { return num_eggs; };
    void SetScore(int n) override { num_eggs = n; };

    void SetTeam(int team) { team_id = team; }
    int GetTeam() const { return team_id; }

	bool IsAlive() { return alive; }
	void SetAlive(bool a) { alive = a; }

private:
    // AnimationPlayer::Listener
    virtual void OnFinish() override;
    void SetRelativeCamPosition(glm::vec3 relativePos);
    void CalculateCameraPosition();
    void CalculateCameraFront();
    float DistanceFromLastPos(glm::vec3 newPosition) const;

private:
    // Player is made up of a model with a camera following it
    std::unique_ptr<Camera> camera;
    //std::unique_ptr<Model> model;
    std::unique_ptr<Animation::AnimatedModel> m_model;

    // Game data
    int id;
    int team_id;
    int num_eggs;

    Texture* info_panel;

    // How up/down camera is
    float camAngle;
    glm::vec3 relativeCamPosition;
    glm::vec3 defaultCamFront;
    glm::vec3 relativeCamPerpendicular;

    float m_HViewSensitivity = 7.5f;
    float m_VViewSensitivity = 5.0f;

    int tick = 0;

	bool alive;

    // Path name for chicken model texture
    std::string modelFile;

    // Animation
    State m_state;
    float m_lastTime_t;     // Test
    float m_distanceThreshhold_t;
    glm::vec3 m_lastPos_t;

};

