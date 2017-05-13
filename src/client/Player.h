#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stack>
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

enum class Direction
{
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down,
};


class Player : public Entity, public Animation::AnimationPlayer::Listener
{
public:
    enum class State
    {
        Idle,
        Jump,
        Walk,
        Dance,
        Attack,
        Death,
        Taunt,
        Peck,
    };

    Player(float x = 0.0f,
           float y = 0.0f,
           float z = 0.0f,
           float rotW = 0.0f,
           float rotX = 0.0f,
           float rotY = 0.0f,
           float rotZ = 0.0f);
    Player(int client_id);
    ~Player();

    // Inherited via Entity
    virtual void Update(float deltaTime) override;
    virtual void Draw() const override;
    virtual void UseShader() const override;
    virtual void SetShaderUniforms() const override;

    virtual void MoveTo(float x, float y, float z) override;
    virtual void RotateTo(const glm::quat& newOrientation) override;
    void Jump() { ChangeState(State::Jump); }
    void Dance() { ChangeState(State::Dance); }
    void Attack() { ChangeState(State::Attack); }
    void TauntDie() { ChangeState(State::Death); }
    void Die() { ChangeState(State::Death); m_alive = false; m_health = 0; }
    void Peck() { ChangeState(State::Peck); }


    void SetModelFile(std::string fileName);

    // Process movement
    void ProcessKeyboard(Direction direction, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void ProcessViewMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);

    glm::vec3 CameraPosition() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetPerspectiveMatrix() const;
	glm::vec3 GetFront() const;
    glm::mat3 GetNormalMatrix() const;
	float GetCamAngle() const;

    int GetID() const { return m_id; };
    int GetClassId() const { return m_classId; }

    void ChangeState(State state);                  // Will change model state and player state
    void SetState(State state) { m_state = state; }     // Simply Sets the state without changing the model

    int GetScore() const override { return m_numEggs; };
    void SetScore(int n) override { m_numEggs = n; };

	void SetTeam(int team);
    int GetTeam() const { return m_teamId; }

	int GetWeapon();
	void SetWeapon(int m_weapon);

	bool IsAlive() { return m_alive; }
	void SetAlive(bool a) { m_alive = a; m_health = 100; }

	void SetHealth(int h) override { m_health = h; }
	int GetHealth() const override { return m_health; };

private:
    // AnimationPlayer::Listener
    virtual void OnFinish() override;
    void CheckStopDanceSound();
    void SetRelativeCamPosition(glm::vec3 relativePos);
    void CalculateCameraPosition();
    void CalculateCameraFront();
    float DistanceFromLastPos(glm::vec3 newPosition) const;
    void SetAudioListener() const;

private:
    // Player is made up of a model with a camera following it
    std::unique_ptr<Camera> camera;
    //std::unique_ptr<Model> model;
    std::unique_ptr<Animation::AnimatedModel> m_model;

    // Game data
    int m_id;
    int m_teamId;
    int m_numEggs = 0;
	int m_weapon = -1;

    Texture* info_panel;

    // How up/down camera is
    float m_camAngle = 0.0f;
    glm::vec3 m_relativeCamPosition;
    glm::vec3 m_defaultCamFront;
    glm::vec3 m_relativeCamPerpendicular;

    float m_HViewSensitivity = 7.5f;
    float m_VViewSensitivity = 5.0f;

    int m_tick = 0;

	bool m_alive = true;
	int m_health = 100;

    // Path name for chicken model texture
    std::string m_modelFile;

    // Animation
    State m_state;
    float m_lastTime_t;     // Test
    float m_distanceThreshhold_t = 1.0f;
    glm::vec3 m_lastPos_t;

    std::stack<int> m_danceSoundIndices;      // Sound index

};

