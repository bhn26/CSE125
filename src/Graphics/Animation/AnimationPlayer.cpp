#include "AnimationPlayer.h"
#include "AnimationMesh.h"

#include <assimp/scene.h>
#include <assimp/anim.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include <cstdio>

namespace Animation
{
    ///////////////////////////////////////////////////////////////////////
    // Constants
    Channel Channel::INVALID_CHANNEL = Channel();

    ///////////////////////////////////////////////////////////////////////
    // Constructor
    Channel::Channel(const aiNodeAnim* nodeAnim) : Channel()
    {
        // Parse position keys
        for (unsigned int i = 0; i < nodeAnim->mNumPositionKeys; i++)
        {
            VectorKey posKey;
            posKey.m_time = (float)nodeAnim->mPositionKeys[i].mTime;
            posKey.m_value = glm::vec3(nodeAnim->mPositionKeys[i].mValue.x,
                                       nodeAnim->mPositionKeys[i].mValue.y,
                                       nodeAnim->mPositionKeys[i].mValue.z);
            m_positionKeys.push_back(std::move(posKey));
        }
        // Parse scaling keys
        for (unsigned int i = 0; i < nodeAnim->mNumScalingKeys; i++)
        {
            VectorKey scaleKey;
            scaleKey.m_time = (float)nodeAnim->mScalingKeys[i].mTime;
            scaleKey.m_value = glm::vec3(nodeAnim->mScalingKeys[i].mValue.x,
                                         nodeAnim->mScalingKeys[i].mValue.y,
                                         nodeAnim->mScalingKeys[i].mValue.z);
            m_scaleKeys.push_back(std::move(scaleKey));
        }
        // Parse rotation keys
        for (unsigned int i = 0; i < nodeAnim->mNumRotationKeys; i++)
        {
            QuatKey rotKey;
            rotKey.m_time = (float)nodeAnim->mScalingKeys[i].mTime;
            rotKey.m_value = glm::quat(nodeAnim->mRotationKeys[i].mValue.w,
                                       nodeAnim->mRotationKeys[i].mValue.x,
                                       nodeAnim->mRotationKeys[i].mValue.y,
                                       nodeAnim->mRotationKeys[i].mValue.z);
            m_rotationKeys.push_back(std::move(rotKey));
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // Evaluate a channel. Does manual matrix multiplication for
    // specialized scaling and transformation matrices with mostly 0s
    glm::mat4 Channel::Evaluate(float time, float duration) const
    {
        glm::vec3 scaling = CalcInterpolation(m_scaleKeys, time, duration, m_lastScaleKey);
        glm::quat rotationQ = CalcInterpolation(m_rotationKeys, time, duration, m_lastRotKey);
        glm::vec3 translation = CalcInterpolation(m_positionKeys, time, duration, m_lastPosKey);

        // Generate return matrix
        glm::mat4 ret = static_cast<glm::mat4>(glm::quat(rotationQ));

        // Multiply by scaling
        const static int VEC3_SIZE = 3;
        for (int col = 0; col < VEC3_SIZE; col++)
            for (int row = 0; row < VEC3_SIZE; row++)
                ret[col][row] *= scaling[col];

        // Add translation
        ret[3][0] = translation.x;
        ret[3][1] = translation.y;
        ret[3][2] = translation.z;

        return ret;
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns an invalid channel if doesn't exist by name
    const Channel& Animation::GetChannel(std::string name) const
    {
        auto it = m_channelMap.find(name);
        return (it != m_channelMap.end()) ? m_channels[it->second] : Channel::INVALID_CHANNEL;
    }

    ///////////////////////////////////////////////////////////////////////
    // Constructor from an assimp animation object
    Animation::Animation(const aiAnimation* animation, bool loops)
        : m_duration((float)animation->mDuration)
        , m_ticksPerSecond((float)animation->mTicksPerSecond)
        , m_loops(loops)
    {
        // Create each channel and add to vector
        for (unsigned int i = 0; i < animation->mNumChannels; i++)
        {
            Channel channel = Channel(animation->mChannels[i]);
            m_channels.push_back(std::move(channel));
            m_channelMap[std::string(animation->mChannels[i]->mNodeName.C_Str())] =
                i; // Map name -> index in channelMap
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // Set the 0 time transforms
    void AnimationPlayer::InitBones0() { EvaluateChildren(m_mesh->m_rootNode, 0.0f); }

    ///////////////////////////////////////////////////////////////////////
    // Add an animation from the assimp scene. Will not override any
    // previous animation. Uses The name in the aiAnimation object
    std::string
        AnimationPlayer::AddAnimFromScene(const aiScene* scene, bool loops, std::string animName)
    {
        if (scene->mNumAnimations < 1)
        {
            fprintf(stderr, "No animations in assimp scene!\n");
            return "";
        }

        aiAnimation& aiAnim = *scene->mAnimations[0];
        std::string name = (animName.length() == 0) ? std::string(aiAnim.mName.C_Str()) : animName;
        if (m_animMap.find(name) != m_animMap.end()) // Don't override an animation
        {
            fprintf(stderr, "Will not override animation: %s\n", name.c_str());
            do
            {
                name[name.length() - 1]++;
            } while (m_animMap.find(name) != m_animMap.end());
            // return "";
        }

        Animation animation = Animation(scene->mAnimations[0], loops); // Pass in the only animation
        m_animMap[name] = m_animations.size();
        m_animations.push_back(std::move(animation));
        return name;
    }

    ///////////////////////////////////////////////////////////////////////
    // Start playing the given animation from the beginning.
    bool AnimationPlayer::PlayAnimation(const std::string& name)
    {
        SetAnimation(name);
        m_animating = true; // Set to update and evaluate
        return true;
    }

    ///////////////////////////////////////////////////////////////////////
    // Sets the current animation
    bool AnimationPlayer::SetAnimation(const std::string& name)
    {
        auto it = m_animMap.find(name);
        if (it == m_animMap.end())
        {
            fprintf(stderr, "No animation exists: %s\n", name.c_str());
            return false;
        }
        m_currAnimationIndex = it->second;
        m_playTimer = 0.0f;
        return true;
    }

    ///////////////////////////////////////////////////////////////////////
    // Updates the time on the player if animating
    void AnimationPlayer::Update(float delta)
    {
        if (m_animating)
        {
            m_playTimer += delta;
            Evaluate();
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // Toplevel Evaluate call. Will calculate the time in ticks and then
    // call evaluate on the root node
    void AnimationPlayer::Evaluate()
    {
        const Animation& currAnim = m_animations[m_currAnimationIndex];
        float ticksPerSecond = (currAnim.m_ticksPerSecond != 0) ? currAnim.m_ticksPerSecond : 25.0f;
        float timeInTicks = m_playTimer * ticksPerSecond; // Timer in seconds
        if (timeInTicks > currAnim.m_duration)
        {
            if (currAnim.m_loops)
                timeInTicks = fmod(timeInTicks, currAnim.m_duration);
            else
            {
                Finish();
                // m_animating = false;
                return;
            }
        }

        EvaluateChildren(m_mesh->m_rootNode, timeInTicks);
    }

    ///////////////////////////////////////////////////////////////////////
    // Recursive Evaluate. Give the node to evaluate at given time in ticks
    // with parent transform
    void AnimationPlayer::EvaluateChildren(const std::unique_ptr<Mesh::Node>& node,
                                           float time,
                                           glm::mat4 parentTransform) const
    {
        const Channel& channel = m_animations[m_currAnimationIndex].GetChannel(node->m_name);

        glm::mat4 globalTransform =
            parentTransform
            * (channel.Valid()
                   ? channel.Evaluate(time, m_animations[m_currAnimationIndex].m_duration)
                   : node->m_nodeTransform);
        if (m_mesh->m_boneMapping.find(node->m_name) != m_mesh->m_boneMapping.end())
        {
            Mesh::BoneInfo& boneInfo = m_mesh->m_boneInfo[m_mesh->m_boneMapping[node->m_name]];
            boneInfo.m_finalTransformation =
                m_mesh->m_globalInverseTransform * globalTransform * boneInfo.m_boneOffset;
        }

        for (std::unique_ptr<Mesh::Node>& child : node->m_children)
        {
            EvaluateChildren(child, time, globalTransform);
        }
    }

    ///////////////////////////////////////////////////////////////////////
    void AnimationPlayer::OnFinish()
    {
        for (Listener* listener : m_listeners)
            listener->OnFinish();
    }
}
