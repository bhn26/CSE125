//
//  Chicken.cpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "ChickenAnim.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <string>
#include <memory>

#include "../Camera.h"
#include "../Scene.h"
#include "../PointLight.h"

ChickenAnim::ChickenAnim() : m_toWorld(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f)))
{
    m_object.FBXLoadClean("assets/chickens/animations/hardcore_chicken.fbx", true);
    m_directionalLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_directionalLight.ambientIntensity = 0.55f;
    m_directionalLight.diffuseIntensity = 0.9f;
    m_directionalLight.direction = glm::vec3(1.0f, 0.0, 0.0);

    SkinningTechnique* skinTechnique = m_object.GetMesh().GetSkinningTechnique();

    skinTechnique->Enable();

    skinTechnique->SetColorTextureUnit(0); // color texture unit index = 0
    skinTechnique->SetDirectionalLight(m_directionalLight);
    skinTechnique->SetMatSpecularIntensity(0.0f);
    skinTechnique->SetMatSpecularPower(0);

    m_object.InitBones0();  // Initialize bones to 0 time spot
}

ChickenAnim::~ChickenAnim()
{
}

void ChickenAnim::Draw()
{
    SkinningTechnique* skinTechnique = m_object.GetMesh().GetSkinningTechnique();
    skinTechnique->Enable(); // use shader

    skinTechnique->SetEyeWorldPos(Scene::camera->Position());
    skinTechnique->SetWVP(Scene::camera->GetPerspectiveMatrix() * Scene::camera->GetViewMatrix() * m_toWorld);
    skinTechnique->SetWorldMatrix(m_toWorld);

    m_object.Draw();
}

void ChickenAnim::Update()
{
    m_object.Update();
}

void ChickenAnim::Scale(float s)
{
    m_toWorld = m_toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(s));
}


