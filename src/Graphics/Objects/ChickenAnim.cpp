//
//  Chicken.cpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "ChickenAnim.h"
#include "../Scene.h"
#include "../PointLight.h"

ChickenAnim::ChickenAnim() : m_toWorld(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f)))
{
    m_pEffect = NULL;
    m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.55f;
    m_directionalLight.DiffuseIntensity = 0.9f;
    m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);
    
    m_pEffect = new SkinningTechnique();
    
    if (!m_pEffect->Init())
    {
        printf("Error initializing the lighting technique\n");
    }
    
    m_pEffect->Enable();
    
    m_pEffect->SetColorTextureUnit(0); // color texture unit index = 0
    m_pEffect->SetDirectionalLight(m_directionalLight);
    m_pEffect->SetMatSpecularIntensity(0.0f);
    m_pEffect->SetMatSpecularPower(0);
    
    if (!m_mesh.LoadMesh("assets/chickens/animations/chicken_dance.fbx"))
    {
        printf("Mesh load failed\n");
    }

    std::vector<glm::mat4> transforms;
    m_mesh.BoneTransform(0.0f, transforms);

    for (uint i = 0; i < transforms.size(); i++)
    {
        m_pEffect->SetBoneTransform(i, transforms[i]);
    }
}

ChickenAnim::~ChickenAnim()
{
    SAFE_DELETE(m_pEffect);
}

void ChickenAnim::Draw()
{
    m_pEffect->Enable(); // use shader

    m_pEffect->SetEyeWorldPos(Scene::camera->Position());
    m_pEffect->SetWVP(Scene::camera->GetPerspectiveMatrix() * Scene::camera->GetViewMatrix() * m_toWorld);
    m_pEffect->SeViewMatrix(Scene::camera->GetViewMatrix());
    m_pEffect->SetPerspectiveMatrix(Scene::camera->GetPerspectiveMatrix());
    m_pEffect->SetWorldMatrix(m_toWorld);

    m_mesh.Render();
}

void ChickenAnim::Update()
{
    //i think this is crashing at the end of the animation loop
    std::vector<glm::mat4> Transforms;
    
    float RunningTime = GetRunningTime();
    
    m_mesh.BoneTransform(RunningTime, Transforms);
    
    for (uint i = 0 ; i < Transforms.size() ; i++)
    {
        m_pEffect->SetBoneTransform(i, Transforms[i]);
    }
}

