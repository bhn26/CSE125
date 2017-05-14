#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <memory>

#include "Entity.h"
#include "Graphics/ShaderManager.h"

struct DirectionalLight;
class Model;

class StaticObject : public Entity
{
public:
    StaticObject(std::shared_ptr<Model> model);
    ~StaticObject();

    void Translate(const glm::vec3& translate);
    void Rotate(float deg, const glm::vec3& axis);

    std::shared_ptr<Model> m_model;

    float m_angle; // For spinning if we want

    // Inherited via Entity
    void Draw() const override;
    virtual void SetShaderUniforms() const override;

    void Spin(float deg);

private:
};
