//
//  Camera.h
//
//  Created by Michelle Wu on 4/7/16.
//  Copyright (c) 2016 Michelle Wu. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
protected:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraTarget;
    glm::vec3 cameraDirection;
    glm::vec3 up;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;

    glm::mat4 view;
    GLfloat cameraSpeed;
    
public:
    Camera(); // Constructor
    void MoveForward();
    void MoveBack();
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();

    glm::mat4 View() { return this->view; }
};
