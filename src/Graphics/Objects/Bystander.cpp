#include "Bystander.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Graphics/Camera.h"
#include "Graphics/Objects/Chicken.h"
#include "Graphics/Shader.h"
#include "Graphics/Scene.h"
#include "Graphics/Lights.h"
#include "Graphics/Model.h"
#include "Graphics/Animation/AnimatedModel.h"
#include "client/ClientGame.h"
#include "Basic/Utils.h"

//// rendering text about the player
#include "client/Window.h"
#include "client/TextRenderer.h"

////////////////////////////////////////////////////////////////////////////////
Bystander::Bystander(float x, float y, float z, float rotW, float rotX, float rotY, float rotZ) :
	Entity(glm::vec3(x, y, z), glm::vec3(0.01f)), modelFile("assets/chickens/objects/chicken.obj")
{
	// Setup the animated model
	m_model = std::unique_ptr<Animation::AnimatedModel>(new Animation::AnimatedModel);
	m_model->FBXLoadClean("assets/chickens/chicken_dance.fbx", true, "dance");
	m_model->AddAnimation("assets/chickens/chicken_cheer.fbx", true, "cheer");

	m_model->RegisterListener(this);

	Animation::SkinningTechnique* skinTechnique = m_model->GetMesh().GetSkinningTechnique();

	skinTechnique->Enable();

	skinTechnique->SetColorTextureUnit(0); // color texture unit index = 0
	skinTechnique->SetDirectionalLight(*Scene::Instance()->GetDirectionalLight());
	skinTechnique->SetMatSpecularIntensity(0.0f);
	skinTechnique->SetMatSpecularPower(0);

	m_model->InitBones0();  // Initialize bones to 0 time spot
}

////////////////////////////////////////////////////////////////////////////////
Bystander::~Bystander()
{
	//delete info_panel;
}

////////////////////////////////////////////////////////////////////////////////
void Bystander::SetModelFile(std::string fileName) {
	modelFile = fileName;
	//model = std::unique_ptr<Model>(new Model(modelFile.c_str()));
	//m_model = std::unique_ptr<Animation::AnimatedModel>(new Animation::AnimatedModel(fileName));
}

////////////////////////////////////////////////////////////////////////////////
void Bystander::OnFinish()
{
    ChangeState(State::IDLE);
}

////////////////////////////////////////////////////////////////////////////////
void Bystander::Update(float deltaTime)
{
	m_model->Update(deltaTime);
}

////////////////////////////////////////////////////////////////////////////////
void Bystander::Draw() const
{
	// Use the appropriate shader (depth or model)
	UseShader();
	// Draw the loaded model
	m_model->Draw(!Scene::Instance()->IsRenderingDepth());
}

////////////////////////////////////////////////////////////////////////////////
void Bystander::UseShader() const
{
	Animation::SkinningTechnique* skinTechnique = m_model->GetMesh().GetSkinningTechnique();
	skinTechnique->Enable(); // use shader
							 // For rendering Depth, only need model
	if (Scene::Instance()->IsRenderingDepth())
	{
		skinTechnique->SetWorldMatrix(toWorld);
		skinTechnique->SetRenderingDepth(true);
		skinTechnique->SetLightSpaceMatrix(Scene::Instance()->LightSpaceMatrix());
	}
	else
	{
		this->SetShaderUniforms();
	}
}

////////////////////////////////////////////////////////////////////////////////
void Bystander::SetShaderUniforms() const
{
	Animation::SkinningTechnique* skinTechnique = m_model->GetMesh().GetSkinningTechnique();
	skinTechnique->Enable(); // use shader

	skinTechnique->SetDepthMap("shadowMap", Scene::Instance()->ShadowMapIndex(), Scene::Instance()->DepthMap());
	skinTechnique->SetRenderingDepth(false);

	skinTechnique->SetEyeWorldPos(Scene::Instance()->GetCameraPosition());
	skinTechnique->SetWVP(Scene::Instance()->GetPerspectiveMatrix() * Scene::Instance()->GetViewMatrix() * toWorld);
	skinTechnique->SetWorldMatrix(toWorld);
}

////////////////////////////////////////////////////////////////////////////////
void Bystander::ChangeState(State state)
{
	if (state == m_state)
		return;

	SetState(state);
	switch (state)
	{
	case State::IDLE:
		m_model->SetAnimation("dance");
		m_model->Reset();
		break;
	case State::DANCE:
		m_model->PlayAnimation("dance");
		break;
	case State::CHEER:
		m_model->PlayAnimation("cheer");
		break;
	}
}
