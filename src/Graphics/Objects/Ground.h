#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // glm::mat4
#include <memory>

#include "../Shader.h"
#include "Entity.h"

class Ground : public Entity
{
public:
    static const float SIZE;

    glm::vec3 color;

    Ground();
    ~Ground();

    virtual void Draw() const override;

    // Inherited via Entity
    virtual void Update(float deltaTime) override;
};