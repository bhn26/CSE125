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

ChickenAnim::ChickenAnim() : m_toWorld(glm::mat4(1.0f))
{
    m_pEffect = NULL;
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.55f;
    m_directionalLight.DiffuseIntensity = 0.9f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
    
    m_persProjInfo.FOV = 45.0f;
    m_persProjInfo.Height = Window::height;
    m_persProjInfo.Width = Window::width;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 1000.0f;
    
    m_position = Vector3f(0.0f, 0.0f, 6.0f);
    
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

    float RunningTime = GetRunningTime();

    vector<Matrix4f> transforms;
    m_mesh.BoneTransform(RunningTime, transforms);

    for (uint i = 0; i < transforms.size(); i++)
    {
        m_pEffect->SetBoneTransform(i, transforms[i]);
    }
}

ChickenAnim::~ChickenAnim()
{
    SAFE_DELETE(m_pEffect);
}

void ChickenAnim::Draw(Camera* camera)
{
    m_pEffect->Enable(); // use shader

    glm::vec3 pos = Scene::camera->Position();
    m_pEffect->SetEyeWorldPos(Vector3f(pos.x, pos.y, pos.z));
    
    //Pipeline p;
    //p.SetCamera(Scene::camera->Position(), Scene::camera->Position() + Scene::camera->Front(), Scene::camera->WorldUp());
    //p.SetPerspectiveProj(m_persProjInfo);
    //p.Scale(0.1f, 0.1f, 0.1f);
    //
    //Vector3f Pos(m_position);
    //p.WorldPos(Pos);
    //p.Rotate(270.0f, 180.0f, 0.0f);
    //m_pEffect->SetWVP(p.GetWVPTrans());
    //m_pEffect->SetWorldMatrix(p.GetWorldTrans());

    m_pEffect->SetWVP(Scene::camera->GetPerspectiveMatrix() * Scene::camera->GetViewMatrix() * m_toWorld);
    m_pEffect->SetWorldMatrix(m_toWorld);

    m_mesh.Render();
}

void ChickenAnim::Update()
{
    //i think this is crashing at the end of the animation loop
    vector<Matrix4f> Transforms;
    
    float RunningTime = GetRunningTime();
    
    m_mesh.BoneTransform(RunningTime, Transforms);
    
    for (uint i = 0 ; i < Transforms.size() ; i++)
    {
        m_pEffect->SetBoneTransform(i, Transforms[i]);
    }
}

