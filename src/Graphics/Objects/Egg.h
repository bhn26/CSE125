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

//#include "../../Window.h"
//#include "../Model.h"
//#include "../Camera.h"
#include "../Shader.h"
#include "Entity.h"

class Model;

class Egg : public Entity
{
public:
	Egg();
	Egg(float x, float y, float z);
	~Egg();

	Model* model;

	glm::vec3 color;

	void Draw() const override;
	void Update() override;
	void Spin(float deg);
	void SetColor(glm::vec3 color);
		
	// Inherited via Entity
	virtual void Spawn(float x, float y, float z) override;
};

#endif /* Egg_h */
