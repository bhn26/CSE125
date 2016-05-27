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
#include "Client/Player.h"

ChickenAnim::ChickenAnim() : m_toWorld(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f)))
{
    m_toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, 0.0f)) * m_toWorld;

    //m_model.FBXLoadClean("assets/chickens/animations/hardcore_chicken.fbx", true);
    m_dance = m_model.FBXLoadClean("assets/chickens/chicken_dance.fbx", true);
    m_walk = m_model.AddAnimation("assets/chickens/chicken_walk.fbx", true);
    m_attack = m_model.AddAnimation("assets/chickens/chicken_peck.fbx", false);
    m_jump = m_model.AddAnimation("assets/chickens/chicken_jump.fbx", false);

    if (!m_walk.length() || !m_jump.length() || !m_attack.length() || !m_dance.length())
    {
        fprintf(stderr, "ERROR:\n\tWalk: %s\n\tJump: %s\n\tAttack: %s\n\tDance: %s\n", m_walk.c_str(), m_jump.c_str(), m_attack.c_str(), m_dance.c_str());
    }

    m_directionalLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_directionalLight.ambientIntensity = 0.55f;
    m_directionalLight.diffuseIntensity = 0.9f;
    m_directionalLight.direction = glm::vec3(1.0f, 0.0, 0.0);

    SkinningTechnique* skinTechnique = m_model.GetMesh().GetSkinningTechnique();

    skinTechnique->Enable();

    skinTechnique->SetColorTextureUnit(0); // color texture unit index = 0
    skinTechnique->SetDirectionalLight(m_directionalLight);
    skinTechnique->SetMatSpecularIntensity(0.0f);
    skinTechnique->SetMatSpecularPower(0);

    m_model.InitBones0();  // Initialize bones to 0 time spot
}

void ChickenAnim::Draw() const
{
    SkinningTechnique* skinTechnique = m_model.GetMesh().GetSkinningTechnique();
    skinTechnique->Enable(); // use shader

    skinTechnique->SetEyeWorldPos(Scene::Instance()->GetCameraPosition());
    skinTechnique->SetWVP(Scene::Instance()->GetPerspectiveMatrix() * Scene::Instance()->GetViewMatrix() * m_toWorld);
    skinTechnique->SetWorldMatrix(m_toWorld);
    m_model.Draw();
}

void ChickenAnim::Update(float deltaTime)
{
    m_model.Update(deltaTime);
}

void ChickenAnim::Scale(float s)
{
    m_toWorld = m_toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(s));
}

void ChickenAnim::Walk()
{
    m_model.PlayAnimation(m_walk);
}

void ChickenAnim::Attack()
{
    m_model.PlayAnimation(m_attack);
}

void ChickenAnim::Jump()
{
    m_model.PlayAnimation(m_jump);
}

void ChickenAnim::Dance()
{
    m_model.PlayAnimation(m_dance);
}
