//
//  Chicken.cpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Chicken.h"
#include "../Scene.h"
#include "../Lights.h"
#include "../Camera.h"
#include "../Model.h"
#include "../ModelManager.h"

#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Chicken::Chicken() : Chicken(0.0f, 0.0f, 0.0f)
{
}

Chicken::Chicken(float x, float y, float z) : Entity(glm::vec3(x, y, z))
{
    m_model = ModelManager::GetModel("Chicken");
}

Chicken::~Chicken()
{
}

void Chicken::Draw() const
{
    // Use the appropriate m_shader (depth or m_model)
    UseShader();

    // Draw the loaded m_model
    m_model->Draw(Scene::Instance()->IsRenderingDepth() ? nullptr : m_shader.get());
}

void Chicken::SetShaderUniforms() const
{
    glUniformMatrix4fv(
        m_shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(m_shader->GetUniform("model"), 1, false, glm::value_ptr(m_toWorld));
    glUniformMatrix4fv(m_shader->GetUniform("projection"),
                       1,
                       false,
                       glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
}
