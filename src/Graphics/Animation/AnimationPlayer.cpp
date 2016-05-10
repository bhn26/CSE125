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
            posKey._time = (float)nodeAnim->mPositionKeys[i].mTime;
            posKey._value = glm::vec3(nodeAnim->mPositionKeys[i].mValue.x,
                                      nodeAnim->mPositionKeys[i].mValue.y,
                                      nodeAnim->mPositionKeys[i].mValue.z);
            _positionKeys.push_back(std::move(posKey));
        }
        // Parse scaling keys
        for (unsigned int i = 0; i < nodeAnim->mNumScalingKeys; i++)
        {
            VectorKey scaleKey;
            scaleKey._time = (float)nodeAnim->mScalingKeys[i].mTime;
            scaleKey._value = glm::vec3(nodeAnim->mScalingKeys[i].mValue.x,
                                        nodeAnim->mScalingKeys[i].mValue.y,
                                        nodeAnim->mScalingKeys[i].mValue.z);
            _scaleKeys.push_back(std::move(scaleKey));
        }
        // Parse rotation keys
        for (unsigned int i = 0; i < nodeAnim->mNumRotationKeys; i++)
        {
            QuatKey rotKey;
            rotKey._time = (float)nodeAnim->mScalingKeys[i].mTime;
            rotKey._value = glm::quat(nodeAnim->mRotationKeys[i].mValue.w,
                                      nodeAnim->mRotationKeys[i].mValue.x,
                                      nodeAnim->mRotationKeys[i].mValue.y,
                                      nodeAnim->mRotationKeys[i].mValue.z);
            _rotationKeys.push_back(std::move(rotKey));
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // Evaluate a channel. Does manual matrix multiplication for
    // specialized scaling and transformation matrices with mostly 0s
    glm::mat4 Channel::Evaluate(float time, float duration) const
    {
        glm::vec3 scaling = CalcInterpolation(_scaleKeys, time, duration, _lastScaleKey);
        glm::quat rotationQ = CalcInterpolation(_rotationKeys, time, duration, _lastRotKey);
        glm::vec3 translation = CalcInterpolation(_positionKeys, time, duration, _lastPosKey);

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
        auto it = _channelMap.find(name);
        return (it != _channelMap.end()) ? _channels[it->second] : Channel::INVALID_CHANNEL;
    }

    ///////////////////////////////////////////////////////////////////////
    // Constructor from an assimp animation object
    Animation::Animation(const aiAnimation* animation, bool loops) : _duration((float)animation->mDuration),
        _ticksPerSecond((float)animation->mTicksPerSecond), _loops(loops)
    {
        // Create each channel and add to vector
        for (unsigned int i = 0; i < animation->mNumChannels; i++)
        {
            Channel channel = Channel(animation->mChannels[i]);
            _channels.push_back(std::move(channel));
            _channelMap[std::string(animation->mChannels[i]->mNodeName.C_Str())] = i;   // Map name -> index in channelMap
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // Set the 0 time transforms
    void AnimationPlayer::InitBones0()
    {
        EvaluateChildren(m_mesh->m_rootNode, 0.0f);
    }

    ///////////////////////////////////////////////////////////////////////
    // Add an animation from the assimp scene. Will not override any 
    // previous animation. Uses The name in the aiAnimation object
    bool AnimationPlayer::AddAnimFromScene(const aiScene* scene, bool loops)
    {
        if (scene->mNumAnimations < 1)
        {
            fprintf(stderr, "No animations in assimp scene!\n");
            return false;
        }

        aiAnimation& aiAnim = *scene->mAnimations[0];
        std::string name = std::string(aiAnim.mName.C_Str());
        if (m_animMap.find(name) != m_animMap.end())      // Don't override an animation
        {
            fprintf(stderr, "Will not override animation: %s\n", name.c_str());
            return false;
        }

        Animation animation = Animation(scene->mAnimations[0], loops);      // Pass in the only animation
        m_animMap[name] = m_animations.size();
        m_animations.push_back(std::move(animation));
        return true;
    }

    ///////////////////////////////////////////////////////////////////////
    // Start playing the given animation from the beginning.
    bool AnimationPlayer::PlayAnimation(std::string name)
    {
        auto it = m_animMap.find(name);
        if (it == m_animMap.end())
        {
            fprintf(stderr, "No animation exists: %s\n", name.c_str());
            return false;
        }
        m_currAnimationIndex = it->second;
        m_playTimer = 0.0f;
        m_animating = true;     // Set to update and evaluate
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
        float ticksPerSecond = (currAnim._ticksPerSecond != 0) ? currAnim._ticksPerSecond : 25.0f;
        float timeInTicks = m_playTimer/1000 * ticksPerSecond;
        if (timeInTicks > currAnim._duration)
        {
            if (currAnim._loops)
                timeInTicks = fmod(timeInTicks, currAnim._duration);
            else
            {
                m_animating = false;
                return;
            }
        }

        EvaluateChildren(m_mesh->m_rootNode, timeInTicks);
    }

    ///////////////////////////////////////////////////////////////////////
    // Recursive Evaluate. Give the node to evaluate at given time in ticks
    // with parent transform
    void AnimationPlayer::EvaluateChildren(const std::unique_ptr<Mesh::Node>& node, float time, glm::mat4 parentTransform) const
    {
        const Channel& channel = m_animations[m_currAnimationIndex].GetChannel(node->_name);

        glm::mat4 globalTransform = parentTransform * (channel.Valid() ? channel.Evaluate(time, m_animations[m_currAnimationIndex]._duration) : node->_nodeTransform);
        if (m_mesh->m_boneMapping.find(node->_name) != m_mesh->m_boneMapping.end())
        {
            Mesh::BoneInfo& boneInfo = m_mesh->m_boneInfo[m_mesh->m_boneMapping[node->_name]];
            boneInfo.finalTransformation = m_mesh->m_GlobalInverseTransform * globalTransform * boneInfo.boneOffset;
        }

        for (std::unique_ptr<Mesh::Node>& child : node->_children)
        {
            EvaluateChildren(child, time, globalTransform);
        }
    }
}
