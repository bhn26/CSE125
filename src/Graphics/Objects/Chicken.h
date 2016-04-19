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

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/vec3.hpp>
#include <memory>

#include "../../Window.h"
#include "../Model.h"
#include "../Camera.h"

class Chicken
{
public:
    Chicken();
    ~Chicken();
    
    std::shared_ptr<Shader> shader;
    Model* model;

    glm::mat4 toWorld;
    glm::mat3 normalMatrix;
    float angle;
    
    void Draw(Camera * camera);
    void Update();
    void Spin(float deg);
};

#endif /* Chicken_h */
