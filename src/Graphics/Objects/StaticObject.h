#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <memory>

#include "Entity.h"
#include "../ShaderManager.h"

class Model;

class StaticObject : public Entity
{
public:
    StaticObject(const GLchar* path);
    ~StaticObject();

    void Translate(glm::vec3 translate);
    void Scale(float scaleFactor);
    void Rotate(float deg, glm::vec3 axis);

    Model* model;

    float angle; // For spinning if we want

    void Draw() const override;
    void Update(float deltaTime) override;
    void Spin(float deg);
};
