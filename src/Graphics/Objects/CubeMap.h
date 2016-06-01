//
//  CubeMap.h
//  egg scramble
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "Entity.h"

class CubeMap : public Entity
{
public:
    CubeMap();
    ~CubeMap();
    
    GLuint textureID;
    std::vector<const GLchar*> faces;
    
    GLuint LoadCubeMap();

    // Inherited via Entity
    virtual void Draw() const override;
    virtual void Update(float deltaTime) override {}
    virtual void SetShaderUniforms() const override;

};
