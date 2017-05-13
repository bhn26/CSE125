#pragma once

#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////
struct BaseLight
{
    glm::vec3 m_color;
    float m_ambientIntensity;
    float m_diffuseIntensity;

    BaseLight(glm::vec3 color = glm::vec3(1.0f), float ambient = 0.1f, float diffuse = 1.0f)
        : m_color(color), m_ambientIntensity(ambient), m_diffuseIntensity(diffuse)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////
struct PointLight : public BaseLight
{
    glm::vec3 m_position;

    PointLight(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f))
        : BaseLight(color), m_position(pos)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////
struct DirectionalLight : public BaseLight
{
    glm::vec3 m_direction;

    DirectionalLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
                     glm::vec3 color = glm::vec3(1.0f))
        : BaseLight(color), m_direction(direction)
    {
    }
};