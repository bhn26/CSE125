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
	Grass();
	~Grass();

    //Model* grass;
    std::shared_ptr<Model> grass;
	glm::mat4* modelMatrices;
	GLuint amount;

	glm::vec3 color;
	static GLfloat deltaTime;
	static GLfloat lastFrame;
	void Draw() const override;
	void Update(float deltaTime) override;
	void SetColor(glm::vec3 color);
};

