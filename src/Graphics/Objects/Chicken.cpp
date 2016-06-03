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

Chicken::Chicken() : Chicken (0.0f, 0.0f, 0.0f)
{
}

Chicken::Chicken(float x, float y, float z) : Entity(glm::vec3(x, y, z))
{
    model = ModelManager::GetModel("Chicken");
}

Chicken::~Chicken()
{
}

void Chicken::Draw() const
{
    // Use the appropriate shader (depth or model)
    UseShader();

    // Draw the loaded model
    model->Draw(Scene::Instance()->IsRenderingDepth() ? nullptr : shader.get());
}

void Chicken::SetShaderUniforms() const
{
    glUniformMatrix4fv(shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix4fv(shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
}
