//
//  Chicken.hpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef Chicken_h
#define Chicken_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <memory>

#include "Entity.h"

class Model;

class Chicken : public Entity
{
public:
    Chicken();
    Chicken(float x, float y, float z);
    ~Chicken();
    
    Model* model;

    void Draw() const override;
    void Update(float deltaTime) override;

    // Inherited via Entity
    virtual void Spawn(float x, float y, float z) override;
};

#endif /* Chicken_h */
