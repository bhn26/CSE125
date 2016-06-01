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

class InstanceObject : public Entity
{
public:
	//InstanceObject(const GLchar* path, GLuint num, GLfloat delta);
	InstanceObject(std::shared_ptr<Model> instanceModel, GLuint num, GLfloat delta);
	~InstanceObject();

    //Model* instance;
    std::shared_ptr<Model> instance;
	glm::mat4* modelMatrices;
	GLuint amount;

	glm::vec3 color;
	static GLfloat deltaTime;
	static GLfloat lastFrame;

    // Inherited via Entity
    void Draw() const override;
    void Update(float deltaTime) override {}
    virtual void SetShaderUniforms() const override;

    void SetColor(glm::vec3 color);
};

