//
//  Chicken.hpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef Chicken_h
#define Chicken_h

#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/vec3.hpp>

#include "../Model.h"

class Chicken {
public:
    Chicken();
    ~Chicken();
    
    Shader shader;
    Model * model;
    
    void Draw(glm::mat4 C);
};

#endif /* Chicken_h */
