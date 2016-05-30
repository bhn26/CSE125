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

StaticObject::StaticObject(const GLchar* path, float x, float y, float z) : Entity(x, y, z)
{
    model = new Model(path);
    shader = ShaderManager::GetShader("Model");
}

StaticObject::~StaticObject()
{
	delete(model);
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
	shader->Use();

	// Draw the loaded model
    glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(shader->GetUniform("normalMatrix"), 1, false, glm::value_ptr(this->normalMatrix));
    glUniformMatrix4fv(shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
    LoadDirectionalLight(Scene::Instance()->GetDirectionalLight());

	model->Draw(shader.get());
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

void StaticObject::LoadDirectionalLight(DirectionalLight * dLight) const
{
    glUniform3fv(shader->GetUniform("dLight._base._color"), 1, glm::value_ptr(dLight->_color));
    glUniform1f(shader->GetUniform("dLight._base._ambientIntensity"), dLight->_ambientIntensity);
    glUniform1f(shader->GetUniform("dLight._base._diffuseIntensity"), dLight->_diffuseIntensity);
    glUniform3fv(shader->GetUniform("dLight._direction"), 1, glm::value_ptr(dLight->_direction));
}
