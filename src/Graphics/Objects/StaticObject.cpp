#include "StaticObject.h"
#include "../Scene.h"
#include "../PointLight.h"
#include "../Camera.h"
#include "../Model.h"
#include "../Shader.h"

#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

StaticObject::StaticObject(const GLchar* path)
{
	this->toWorld = glm::mat4(1.0f);
	model = new Model(path);
	//std::shared_ptr<Shader> modelShader = std::make_shared<Shader>("src/Graphics/Shaders/model_loading.vert", "src/Graphics/Shaders/model_loading.frag");
	shader = Scene::Instance()->modelShader;
}

StaticObject::~StaticObject()
{
	delete(model);
}

void StaticObject::Translate(glm::vec3 translate)
{
	this->toWorld = glm::translate(glm::mat4(1.0f), translate) * this->toWorld;
}

void StaticObject::Scale(float scaleFactor)
{
	this->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor)) * this->toWorld;
}

void StaticObject::Rotate(float deg, glm::vec3 axis)
{
	this->toWorld = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), axis) * this->toWorld;
}

void StaticObject::Draw() const
{
	shader->Use();

	// Draw the loaded model
	GLint viewLoc = shader->GetUniform("view");
	GLint modelLocation = shader->GetUniform("model");
	GLint projectionLocation = shader->GetUniform("projection");

	glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
	glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
	glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

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
	this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void StaticObject::Spawn(float x, float y, float z)
{
}
