//
//  Chicken.cpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Chicken.h"
#include "../Scene.h"
#include "../PointLight.h"
#include "../Camera.h"
#include "../Model.h"

#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Chicken::Chicken()
{
    this->toWorld = glm::mat4(1.0f);
  
    model = new Model("assets/chickens/objects/chicken.obj");

    if (!PlaySound("assets/audio/chicken_dance.wav"))
    {
        fprintf(stderr, "Error! Can't play audio file: assets/audio/chicken_dance.wav");
    }
}

Chicken::~Chicken()
{
    delete(model);
}

void Chicken::Draw() const
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

void Chicken::Update()
{
}

void Chicken::Spawn(float x, float y, float z)
{
}
