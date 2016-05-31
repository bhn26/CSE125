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


    // Inherited via Entity
    virtual void Draw() const override;
    virtual void Update(float deltaTime) override;
    virtual void SetShaderUniforms() const override;
};