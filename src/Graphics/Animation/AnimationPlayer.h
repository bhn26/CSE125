#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <unordered_map>
#include <memory>

#include "AnimationMesh.h"

struct aiScene;
struct aiNodeAnim;
struct aiAnimation;

namespace Animation
{
    ///////////////////////////////////////////////////////////////////////
    // Keyframe in a channel
    template <class T> struct Key
    {
        float m_time;
        T m_value;

        Key() : m_time(0.0f) {}
    };
    typedef Key<glm::vec3> VectorKey;
    typedef Key<glm::quat> QuatKey;

    ///////////////////////////////////////////////////////////////////////
    // Channel for a single bone
    struct Channel
    {
        std::vector<VectorKey> m_positionKeys;
        std::vector<VectorKey> m_scaleKeys;
        std::vector<QuatKey> m_rotationKeys;
        int m_lastPosKey = 0, m_lastScaleKey = 0, m_lastRotKey = 0;

        Channel() = default;
        Channel(const aiNodeAnim* nodeAnim);

        glm::mat4 Evaluate(float time, float duration) const;
        bool Valid() const
        {
            return m_positionKeys.size() > 0 && m_scaleKeys.size() > 0 && m_rotationKeys.size() > 0;
        }

        ///////////////////////////////////////////////////////////////////////
        // Templated Methods
        template <typename T> static T Interpolate(T start, T end, float factor);
        template <>
        static glm::quat Interpolate<glm::quat>(glm::quat start, glm::quat end, float factor);
        template <typename T>
        static T
            CalcInterpolation(std::vector<Key<T>> keys, float time, float duration, int lastKey);

        static Channel INVALID_CHANNEL;
    };

    ///////////////////////////////////////////////////////////////////////
    // Animation with channels
    struct Animation
    {
        float m_duration = 0.0f;
        float m_ticksPerSecond = 16.6f;
        bool m_loops = false;
        std::vector<Channel> m_channels;
        std::unordered_map<std::string, int> m_channelMap;

        Animation() = default;
        Animation(const aiAnimation* scene, bool loops = false);
        const Channel& GetChannel(std::string name) const;
    };

    ///////////////////////////////////////////////////////////////////////
    // AnimationPlayer class, manages time, current animation, and modifies
    // the mesh
    class AnimationPlayer
    {
    public:
        ///////////////////////////////////////////////////////////////////////
        // Listener
        class Listener
        {
        public:
            virtual void OnFinish() = 0;
        };

        ///////////////////////////////////////////////////////////////////////
        AnimationPlayer(Mesh* mesh) : m_mesh(mesh) {}

        void InitBones0();

        std::string
            AddAnimFromScene(const aiScene* scene, bool loops = false, std::string animName = "");
        bool PlayAnimation(const std::string& name);
        bool SetAnimation(const std::string& name);

        void Pause() { m_animating = false; }
        void Continue() { m_animating = true; }
        void ToggleAnimation() { m_animating = !m_animating; }
        void RestartAnimation()
        {
            m_animating = true;
            m_playTimer = 0.0f;
        }
        void Reset()
        {
            Pause();
            m_playTimer = 0.0f;
            InitBones0();
        }
        void Finish() { /*Reset();*/ OnFinish(); }

        void Update(float delta);

        void RegisterListener(Listener* listener) { m_listeners.push_back(listener); }

    private:
        void Evaluate();
        void EvaluateChildren(const std::unique_ptr<Mesh::Node>&,
                              float time,
                              glm::mat4 parentTransform = glm::mat4(1.0f)) const;

        void OnFinish();

        Mesh* m_mesh;

        std::vector<Animation> m_animations;
        std::unordered_map<std::string, int> m_animMap;
        std::vector<Listener*> m_listeners;

        int m_currAnimationIndex;
        float m_playTimer; // Timer in seconds
        bool m_animating;
    };

    ///////////////////////////////////////////////////////////////////////
    // Standard Linear Interpolation
    template <typename T> T Channel::Interpolate(T start, T end, float factor)
    {
        return start + (end - start) * factor;
    }

    ///////////////////////////////////////////////////////////////////////
    // Specific interpolate for quaternions, spherical interpolation
    template <>
    glm::quat Channel::Interpolate<glm::quat>(glm::quat start, glm::quat end, float factor)
    {
        return glm::slerp(start, end, factor);
    }

    ///////////////////////////////////////////////////////////////////////
    // Calculate the interpolation between two keys
    template <typename T>
    T Channel::CalcInterpolation(std::vector<Key<T>> keys, float time, float duration, int lastKey)
    {
        if (keys.size() == 1)
        {
            return keys[0].m_value;
        }

        unsigned int key = (time < keys[lastKey].m_time) ? lastKey : 0;

        // Assumes animation is forward playing only
        for (; key < keys.size() - 1; key++)
        {
            if (time < keys[key + 1].m_time)
                break;
        }

        if (time < keys[key].m_time) // HACK. TODO: Use real first frame
        {
            return keys[key].m_value;
        }

        unsigned int nextKey = (key + 1) % keys.size();
        float deltaTime = ((nextKey == 0) ? duration : keys[nextKey].m_time) - keys[key].m_time;
        float factor = (time - keys[key].m_time) / deltaTime;
        assert(factor >= 0.0f && factor <= 1.0f);

        return Interpolate<T>(keys[key].m_value, keys[nextKey].m_value, factor);
    }
}
