#pragma once

#include <string>
#include <glm/glm.hpp>

#include "AnimationMesh.h"
#include "AnimationPlayer.h"

namespace Animation
{
    class AnimatedModel
    {
        ///////////////////////////////////////////////////////////////////////
        // Member variables
        Mesh m_mesh;
        AnimationPlayer m_animPlayer;

        glm::mat4 m_toWorld;

        double lastTime;        // TODO Remove

    public:
        ///////////////////////////////////////////////////////////////////////
        // Public methods
        AnimatedModel();
        AnimatedModel(std::string fbxFilename);

        bool FBXLoadClean(std::string filename, bool animLoops = false);            // Loads both Mesh and Animation
        bool AddAnimation(std::string animationFile, bool loops = false);   // Only loads Animation
        bool PlayAnimation(std::string name);

        void Update();
        void Draw() const;
        void InitBones0() { m_animPlayer.InitBones0(); }
        void ToggleAnimating() { m_animPlayer.ToggleAnimation(); }
        void RestartAnimation() { m_animPlayer.RestartAnimation(); }

        const Mesh& GetMesh() const { return m_mesh; }
        const AnimationPlayer& GetAnimPlayer() const { return m_animPlayer; }
    };
}