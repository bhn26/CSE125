#include "AnimatedModel.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> // Post processing flags

namespace Animation
{
    ///////////////////////////////////////////////////////////////////////
    AnimatedModel::AnimatedModel() : m_animPlayer(&m_mesh)
    {
        double lastTime = (double)GetCurrentTimeMillis();
    }

    AnimatedModel::AnimatedModel(std::string fbxFilename) : AnimatedModel()
    {
        FBXLoadClean(fbxFilename);
    }


    ///////////////////////////////////////////////////////////////////////
    // Load an FBX completely new. Loads mesh and animation
    bool AnimatedModel::FBXLoadClean(std::string filename, bool animLoops)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

        if (!scene)   // If failed, return
            return false;
        
        // Should not fail
        m_mesh.InitFromScene(scene, filename);
        m_animPlayer.AddAnimFromScene(scene, animLoops);

        return true;
    }

    ///////////////////////////////////////////////////////////////////////
    bool AnimatedModel::AddAnimation(std::string animationFile, bool loops)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationFile.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

        if (!scene || !m_mesh.SameMesh(scene))   // If failed, return
        {
            return false;
        }
        
        return m_animPlayer.AddAnimFromScene(scene, loops);
    }

    bool AnimatedModel::PlayAnimation(std::string name)
    {
        return m_animPlayer.PlayAnimation(name);
    }

    ///////////////////////////////////////////////////////////////////////
    void AnimatedModel::Update()
    {
        double nextTime = (double)GetCurrentTimeMillis();
        float delta = (float)(nextTime - lastTime);
        lastTime = nextTime;
        m_animPlayer.Update(delta);
    }

    ///////////////////////////////////////////////////////////////////////
    void AnimatedModel::Draw() const
    {
        m_mesh.Draw();
    }
}