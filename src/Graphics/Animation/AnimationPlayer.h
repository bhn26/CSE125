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
    template<class T> struct Key
    {
        float _time;
        T _value;

        Key() : _time(0.0f) {}
    };
    typedef Key<glm::vec3> VectorKey;
    typedef Key<glm::quat> QuatKey;

    ///////////////////////////////////////////////////////////////////////
    // Channel for a single bone
    struct Channel
    {
        std::vector<VectorKey> _positionKeys;
        std::vector<VectorKey> _scaleKeys;
        std::vector<QuatKey> _rotationKeys;
        int _lastPosKey, _lastScaleKey, _lastRotKey;

        Channel() : _lastPosKey(0), _lastScaleKey(0), _lastRotKey(0) {}
        Channel(const aiNodeAnim* nodeAnim);

        glm::mat4 Evaluate(float time, float duration) const;
        bool Valid() const { return _positionKeys.size() > 0 && _scaleKeys.size() > 0 && _rotationKeys.size() > 0; }

        ///////////////////////////////////////////////////////////////////////
        // Templated Methods
        template <typename T> static T Interpolate(T start, T end, float factor);
        template <> static glm::quat Interpolate<glm::quat>(glm::quat start, glm::quat end, float factor);
        template <typename T> static T CalcInterpolation(std::vector<Key<T>> keys, float time, float duration, int lastKey);

        static Channel INVALID_CHANNEL;
    };

    ///////////////////////////////////////////////////////////////////////
    // Animation with channels
    struct Animation
    {
        float _duration;
        float _ticksPerSecond;
        bool _loops;
        std::vector<Channel> _channels;
        std::unordered_map<std::string, int> _channelMap;

        Animation() : _duration(0.0f) {}
        Animation(const aiAnimation* scene, bool loops = false);
        const Channel& GetChannel(std::string name) const;
    };

    ///////////////////////////////////////////////////////////////////////
    // AnimationPlayer class, manages time, current animation, and modifies
    // the mesh
    class AnimationPlayer
    {
    private:
        Mesh* m_mesh;

        std::vector<Animation> m_animations;
        std::unordered_map<std::string, int> m_animMap;
        int m_currAnimationIndex;
        float m_playTimer;
        bool m_animating;

    public:
        AnimationPlayer(Mesh* mesh) : m_mesh(mesh) { }

        void InitBones0();

        std::string AddAnimFromScene(const aiScene* scene, bool loops = false);
        bool PlayAnimation(std::string name);

        void Pause() { m_animating = false; }
        void Continue() { m_animating = true; }
        void ToggleAnimation() { m_animating = !m_animating; }
        void RestartAnimation() { m_animating = true; m_playTimer = 0.0f; }
        void Reset() { Pause(); m_playTimer = 0.0f; InitBones0(); }

        void Update(float delta);

    private:
        void Evaluate();
        void EvaluateChildren(const std::unique_ptr<Mesh::Node>&, float time, glm::mat4 parentTransform = glm::mat4(1.0f)) const;
    };


    ///////////////////////////////////////////////////////////////////////
    // Standard Linear Interpolation
    template <typename T> T Channel::Interpolate(T start, T end, float factor)
    {
        return start + (end - start)*factor;
    }

    ///////////////////////////////////////////////////////////////////////
    // Specific interpolate for quaternions, spherical interpolation
    template <> glm::quat Channel::Interpolate<glm::quat>(glm::quat start, glm::quat end, float factor)
    {
        return glm::slerp(start, end, factor);
    }

    ///////////////////////////////////////////////////////////////////////
    // Calculate the interpolation between two keys
    template <typename T> T Channel::CalcInterpolation(std::vector<Key<T>> keys, float time, float duration, int lastKey)
    {
        if (keys.size() == 1)
        {
            return keys[0]._value;
        }

        unsigned int key = (time < keys[lastKey]._time) ? lastKey : 0;

        // Assumes animation is forward playing only
        for (; key < keys.size()-1; key++)
        {
            if (time < keys[key+1]._time)
                break;
        }

        if (time < keys[key]._time) // HACK. TODO: Use real first frame
        {
            return keys[key]._value;
        }

        unsigned int nextKey = (key + 1) % keys.size();
        float deltaTime = ((nextKey == 0) ? duration : keys[nextKey]._time) - keys[key]._time;
        float factor = (time - keys[key]._time) / deltaTime;
        assert(factor >= 0.0f && factor <= 1.0f);

        return Interpolate<T>(keys[key]._value, keys[nextKey]._value, factor);
    }
}
