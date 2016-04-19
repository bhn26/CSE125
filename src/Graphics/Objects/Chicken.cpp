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

Chicken::Chicken()
{
    this->toWorld = glm::mat4(1.0f);
    this->angle = 0.0f;
  
    model = new Model("assets/chickens/objects/chicken.obj");
    //shader.SetShaders("src/Graphics/Shaders/model_loading.vert",
    //                  "src/Graphics/Shaders/model_loading.frag");
}

Chicken::~Chicken()
{
    delete(model);
}

void Chicken::Draw(Camera* camera)
{
    shader->Use();
    glm::mat4 projection = glm::perspective(camera->Zoom(), (float)Window::width/(float)Window::height, 0.1f, 1000.0f);
    glm::mat4 view = camera->GetViewMatrix();
    
    // Draw the loaded model
    GLint viewLoc = shader->GetUniform("view");
    GLint modelLocation = shader->GetUniform("model");
    GLint projectionLocation = shader->GetUniform("projection");
    
    glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));

    model->Draw(shader.get());
}

void Chicken::Update()
{
    //Spin(0.3f);
}

void Chicken::Spin(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f)
        this->angle = 0.0f;
    
    // This creates the matrix to rotate the cube
    this->toWorld = glm::rotate(glm::mat4(1.0f), glm::radians(this->angle), glm::vec3(0.0f, 1.0f, 0.0f));
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}