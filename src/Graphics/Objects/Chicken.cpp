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

    //PlaySound("assets/audio/chicken_dance.mp3");
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
    GLint normalMatrixLoc = shader->GetUniform("normalMatrix");
    GLint projectionLocation = shader->GetUniform("projection");
    GLint lightColorLoc = shader->GetUniform("lightColor");
    GLint lightPosLoc = shader->GetUniform("lightPos");
    GLint viewPosLoc = shader->GetUniform("viewPos");
    
    glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(normalMatrixLoc, 1, false, glm::value_ptr(this->normalMatrix));
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));
    
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(Scene::pLight->color));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(Scene::pLight->position));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(Scene::camera->Position()));

    model->Draw(shader.get());
}

void Chicken::Update(bool rotX, bool rotY, bool rotZ)
{
    if(rotX) SpinX(0.3f);
    if(rotY) SpinY(0.3f);
    if(rotZ) SpinZ(0.3f);
}

void Chicken::SpinX(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f)
        this->angle = 0.0f;
    
    // This creates the matrix to rotate the cube
    this->toWorld = toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(1.0f, 0.0f, 0.0f));
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Chicken::SpinY(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f)
        this->angle = 0.0f;
    
    // This creates the matrix to rotate the cube
    this->toWorld = toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Chicken::SpinZ(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f)
        this->angle = 0.0f;
    
    // This creates the matrix to rotate the cube
    this->toWorld = toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Chicken::ShiftX(float dist)
{
    this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(dist, 0.0f, 0.0f)) * this->toWorld;
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Chicken::ShiftY(float dist)
{
    this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, dist, 0.0f)) * this->toWorld;
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Chicken::ShiftZ(float dist)
{
    this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist)) * toWorld;
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Chicken::Scale(float scale)
{
    this->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale)) * toWorld;
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Chicken::Orbit(float deg)
{
    this->toWorld = glm::rotate(glm::mat4(1.0f), deg/180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)) * toWorld;
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Chicken::Reset()
{
    this->toWorld = glm::mat4(1.0f);
}
