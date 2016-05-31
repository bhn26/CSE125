#pragma once

#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////
struct BaseLight
{
    glm::vec3 _color;
    float _ambientIntensity;
    float _diffuseIntensity;

    BaseLight(glm::vec3 color = glm::vec3(1.0f), float ambient = 0.1f, float diffuse = 1.5f) :
        _color(color), _ambientIntensity(ambient), _diffuseIntensity(diffuse)
    {}
};

////////////////////////////////////////////////////////////////////////////////
struct PointLight : public BaseLight
{
    glm::vec3 _position;

    PointLight(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f)) : BaseLight(color), _position(pos)
    {}
};

////////////////////////////////////////////////////////////////////////////////
struct DirectionalLight : public BaseLight
{
    glm::vec3 _direction;

    DirectionalLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f)) :
        BaseLight(color), _direction(direction)
    {}
};