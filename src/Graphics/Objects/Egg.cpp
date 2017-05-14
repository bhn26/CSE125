//
//  Chicken.cpp
//  egg scramble
//
//  Created by Michelle on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Egg.h"
#include "Graphics/Scene.h"
#include "Graphics/Lights.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include "Graphics/ModelManager.h"

#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Egg::Egg() : Egg(0.0f, 0.0f, 0.0f, "Egg"){};

Egg::Egg(float x, float y, float z, std::string type) : Entity(glm::vec3(x, y, z))
{
    m_model = ModelManager::GetModel(type);
}

void Egg::SetColor(const glm::vec3& color)
{
    m_color = color;
}

void Egg::SetShaderUniforms() const
{
    glUniformMatrix4fv(
        m_shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(m_shader->GetUniform("model"), 1, false, glm::value_ptr(m_toWorld));
    glUniformMatrix4fv(
        m_shader->GetUniform("normalMatrix"), 1, false, glm::value_ptr(m_normalMatrix));
    glUniformMatrix4fv(m_shader->GetUniform("projection"),
                       1,
                       false,
                       glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
}

Egg::~Egg()
{
}

void Egg::Draw() const
{
    // Use the appropriate m_shader (depth or m_model)
    UseShader();

    // Draw the loaded m_model
    m_model->Draw(Scene::Instance()->IsRenderingDepth() ? nullptr : m_shader.get());
}

void Egg::Update(float deltaTime)
{
    Spin(0.7f);
}

void Egg::Spin(float deg)
{
    // This creates the matrix to rotate the cube
    m_toWorld =
        m_toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    CalculateNormalMatrix();
}
