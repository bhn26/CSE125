//
//  InstanceObject.h
//  egg scramble
//
//  Created by Michelle on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef InstanceObject_h
#define InstanceObject_h

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

class InstanceObject : public Entity
{
public:
	InstanceObject(const GLchar* path, GLuint num);
	~InstanceObject();

	Model* instance;
	glm::mat4* modelMatrices;
	GLuint amount;

	glm::vec3 color;
	static GLfloat deltaTime;
	static GLfloat lastFrame;
	void Draw() const override;
	void Update() override;

	// Inherited via Entity
	virtual void Spawn(float x, float y, float z) override;
};

#endif /* InstanceObject_h */
