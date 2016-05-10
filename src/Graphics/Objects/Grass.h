//
//  Grass.h
//  egg scramble
//
//  Created by Michelle on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef Grass_h
#define Grass_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <memory>

//#include "../../Window.h"
//#include "../Model.h"
//#include "../Camera.h"
#include "../Shader.h"
#include "Entity.h"

class Model;

class Grass : public Entity
{
public:
	Grass();
	~Grass();

	Model* model;
	glm::mat4* modelMatrices;
	GLuint amount;

	glm::vec3 color;

	void Draw() const override;
	void Update() override;
	void SetColor(glm::vec3 color);

	// Inherited via Entity
	virtual void Spawn(float x, float y, float z) override;
};

#endif /* Grass_h */
