//
//  Chicken.cpp
//  egg scramble
//
//  Created by Michelle on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Egg.h"
#include "../Scene.h"
#include "../PointLight.h"
#include "../Camera.h"
#include "../Model.h"

#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Egg::Egg() : Egg(0.0f, 0.0f, 0.0f)
{
}

Egg::Egg(float x, float y, float z) : Entity(glm::vec3(x, y, z))
{
	model = new Model("assets/eggs/objects/egg.obj");
}

void Egg::SetColor(glm::vec3 color)
{
	this->color = color;
}

Egg::~Egg()
{
    delete model;
}

void Egg::Draw() const
{
	shader->Use();

	// Draw the loaded model
	GLint viewLoc = shader->GetUniform("view");
	GLint modelLocation = shader->GetUniform("model");
	GLint projectionLocation = shader->GetUniform("projection");
	GLint objectColorLoc = shader->GetUniform("objectColor");
	GLint lightColorLoc = shader->GetUniform("lightColor");
	GLint lightPosLoc = shader->GetUniform("lightPos");
	GLint viewPosLoc = shader->GetUniform("viewPos");
	/*GLint ambient = shader->GetUniform("ambient");
	GLint diffuse = shader->GetUniform("diffuse");
	GLint specular = shader->GetUniform("specular");
	GLint shininess = shader->GetUniform("shininess");*/

	//glm::vec4 ambient(0.19f, 0.19f, 0.19f, 1.0f);
	//glm::vec4 diffuse(0.50f, 0.50f, 0.50f, 1.0f);
	//glm::vec4 specular(0.508f, 0.508f, 0.508f, 1.0f);
	//float shininess = 51.2f;

	glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
	glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
	glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
	glUniform3fv(objectColorLoc, 1, glm::value_ptr(this->color));
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(Scene::Instance()->GetPointLight()->color));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(Scene::Instance()->GetPointLight()->position));
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(Scene::Instance()->GetCameraPosition()));

	//glUniform4f(ambient, 1, false, glm::value_ptr(ambient));

	model->Draw(shader.get());
}

void Egg::Spin(float deg)
{
	// This creates the matrix to rotate the cube
	this->toWorld = toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
	this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Egg::Spawn(float x, float y, float z)
{
}
