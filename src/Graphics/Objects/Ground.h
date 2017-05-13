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
    static const float s_size;

    glm::vec3 m_color;

    Ground();
    ~Ground();


    // Inherited via Entity
    virtual void Draw() const override;
    virtual void Update(float deltaTime) override;
    virtual void SetShaderUniforms() const override;
};