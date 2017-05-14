//
//  Chicken.hpp
//  egg scramble
//
//  Created by Michelle on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef Egg_h
#define Egg_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <memory>

#include "Entity.h"

class Model;

class Egg : public Entity
{
public:
    Egg();
    Egg(float x, float y, float z, std::string type);
    ~Egg();

    std::shared_ptr<Model> m_model;
    float m_angle; // For spinning if we want

    glm::vec3 m_color;

    // Inherited via Entity
    void Draw() const override;
    void Update(float deltaTime) override;
    virtual void SetShaderUniforms() const override;

    void Spin(float deg);
    void SetColor(const glm::vec3& color);
};

#endif /* Egg_h */