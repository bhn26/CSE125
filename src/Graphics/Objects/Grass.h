//
//  Grass.h
//  egg scramble
//
//  Created by Michelle on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <memory>

#include "Entity.h"

class Model;

class Grass : public Entity
{
public:
    Grass(const GLchar* path, GLuint num);
    ~Grass();

    // Model* grass;
    std::shared_ptr<Model> m_grass;
    glm::mat4* m_modelMatrices;
    GLuint m_amount;

    glm::vec3 m_color;
    static GLfloat s_deltaTime;
    static GLfloat s_lastFrame;

    // Inherited via Entity
    void Draw() const override;
    void Update(float deltaTime) override;
    virtual void SetShaderUniforms() const override;
};
