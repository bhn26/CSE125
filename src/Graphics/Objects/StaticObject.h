#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <memory>

#include "Entity.h"
#include "../ShaderManager.h"

struct DirectionalLight;
class Model;

class StaticObject : public Entity
{
public:
    StaticObject(std::shared_ptr<Model> model);
    ~StaticObject();

    void Translate(glm::vec3 translate);
    void Rotate(float deg, glm::vec3 axis);

    std::shared_ptr<Model> model;

    float angle; // For spinning if we want

    // Inherited via Entity
    void Draw() const override;
    void Update(float deltaTime) override;
    virtual void SetShaderUniforms() const override;

    void Spin(float deg);

private:

};
