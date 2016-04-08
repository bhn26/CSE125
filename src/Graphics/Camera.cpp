//
//  camera.cpp
//  Homework3
//
//  Created by Michelle Wu on 10/21/14.
//  Copyright (c) 2014 Michelle Wu. All rights reserved.
//

#include "camera.h"

#include <stdio.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Objects/Model.h"

Camera::Camera()
{
    cameraSpeed = 0.1f;
    // Position of the camera
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    // Where the camera is pointing at
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // Vector pointing towards camera's positive z-axis
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    // Positive x-axis of the camera space
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::MoveForward()
{
    cameraPos += cameraSpeed * cameraFront;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);       // Refactor
}

void Camera::MoveBack()
{
    cameraPos -= cameraSpeed * cameraFront;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);       // Refactor
}

void Camera::MoveRight()
{
    cameraPos += cameraSpeed * cameraRight;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);       // Refactor
}

void Camera::MoveLeft()
{
    cameraPos -= cameraSpeed * cameraRight;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);       // Refactor
}

void Camera::MoveUp()
{
    cameraPos += cameraSpeed * cameraUp;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);       // Refactor
}

void Camera::MoveDown()
{
    cameraPos -= cameraSpeed * cameraUp;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);       // Refactor
}