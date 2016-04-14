#pragma once

#include <glm/glm.hpp>

struct PointLight
{
    glm::vec3 position;
    glm::vec3 color;

    PointLight(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 col = glm::vec3(1.0f)) : position(pos), color(col) {}
};