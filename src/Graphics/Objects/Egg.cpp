//
//  Chicken.cpp
//  egg scramble
//
//  Created by Michelle on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Egg.h"
#include "../Scene.h"
#include "../Lights.h"
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
	model = new Model("assets/eggs/objects/dino_egg.obj");
}

void Egg::SetColor(glm::vec3 color)
{
	this->color = color;
}

void Egg::SetShaderUniforms() const
{
    glUniformMatrix4fv(shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix4fv(shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
}

Egg::~Egg()
{
    delete model;
}

void Egg::Draw() const
{
    // Use the appropriate shader (depth or model)
    UseShader();

    // Draw the loaded model
    model->Draw(shader.get());
}

void Egg::Update(float deltaTime)
{
	Spin(0.7f);
}

void Egg::Spin(float deg)
{
	// This creates the matrix to rotate the cube
	this->toWorld = toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
	this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}
