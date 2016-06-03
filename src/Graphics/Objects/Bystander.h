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


class Bystander : public Entity, public Animation::AnimationPlayer::Listener
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
		CHEER,
	};

	Bystander(float x = 0.0f, float y = 0.0f, float z = 0.0f,
		float rotW = 0.0f, float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f);
	Bystander(int client_id);
	~Bystander();

	// Inherited via Entity
	virtual void Update(float deltaTime) override;
	virtual void Draw() const override;
	virtual void UseShader() const override;
	virtual void SetShaderUniforms() const override;

	void Cheer() { ChangeState(State::CHEER); }


	void SetModelFile(std::string fileName);

	int GetClassId() const { return class_id; }

	void ChangeState(State state);                  // Will change model state and player state
	void SetState(State state) { m_state = state; }     // Simply Sets the state without changing the model

private:
	//std::unique_ptr<Model> model;
	std::unique_ptr<Animation::AnimatedModel> m_model;

	// Path name for chicken model texture
	std::string modelFile;

	// Animation
	State m_state;

    // Inherited via Listener
    virtual void OnFinish() override;
};

