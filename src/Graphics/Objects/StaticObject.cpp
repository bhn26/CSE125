#include "StaticObject.h"
#include "../Scene.h"
#include "../Lights.h"
#include "../Camera.h"
#include "../Model.h"
#include "../Shader.h"

#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

StaticObject::StaticObject(std::shared_ptr<Model> model, float x, float y, float z) : Entity(x,y,z), model(model)
{
    shader = ShaderManager::Instance()->GetShader("Model");
}

StaticObject::~StaticObject()
{
	//delete model;
}

void StaticObject::Translate(glm::vec3 translate)
{
	//this->toWorld = glm::translate(glm::mat4(1.0f), translate) * this->toWorld;
    this->toWorld[3] = std::move(glm::vec4(translate, 1.0f));
}

void StaticObject::Rotate(float deg, glm::vec3 axis)
{
    //this->toWorld = glm::rotate(glm::mat4(1.0f), glm::radians(deg), axis) * this->toWorld;
    //this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
    RotateTo(glm::mat3(glm::rotate(toWorld, glm::radians(deg), axis)));
}

void StaticObject::Draw() const
{
    // Use the appropriate shader (depth or model)
    UseShader();

    // Draw the loaded model
    model->Draw(Scene::Instance()->IsRenderingDepth() ? nullptr : shader.get());
}

void StaticObject::Update(float deltaTime)
{
	//Spin(0.3f);
}

void StaticObject::Spin(float deg)
{
	// This creates the matrix to rotate the cube
	this->toWorld = toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    CalculateNormalMatrix();
}

void StaticObject::SetShaderUniforms() const
{
    glUniform1i(shader->GetUniform("shadowMap"), Scene::Instance()->ShadowMapIndex());
    glActiveTexture(GL_TEXTURE0 + Scene::Instance()->ShadowMapIndex());
    glBindTexture(GL_TEXTURE_2D, Scene::Instance()->DepthMap());

    glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(shader->GetUniform("normalMatrix"), 1, false, glm::value_ptr(this->normalMatrix));
    glUniformMatrix4fv(shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
    glUniformMatrix4fv(shader->GetUniform("lightSpaceMatrix"), 1, false, glm::value_ptr(Scene::Instance()->LightSpaceMatrix()));
    glUniform1f(shader->GetUniform("alpha"), this->alpha);
    LoadDirectionalLight(Scene::Instance()->GetDirectionalLight());
}
