//
//  Entity.cpp
//  egg scramble
//
//  Created by Phoebe on 4/14/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>

////////////////////////////////////////////////////////////////////////////////
// NOTE: Constructors do not initialize vertex/element buffers, nor shader
Entity::Entity(int oid, int cid) : Entity()
{
    obj_id = oid;
    class_id = cid;
}

////////////////////////////////////////////////////////////////////////////////
Entity::Entity(glm::mat4 world, glm::vec3 scale) : toWorld(world), scale(scale), VBO(0), VAO(0), EBO(0), shader(nullptr)
{
    ApplyScale();
    CalculateNormalMatrix();
}

////////////////////////////////////////////////////////////////////////////////
Entity::Entity(glm::vec3 pos, glm::vec3 scale) : Entity(glm::translate(glm::mat4(1.0f), pos), scale)
{
}

////////////////////////////////////////////////////////////////////////////////
Entity::Entity(float x, float y, float z, float sx, float sy, float sz) : Entity(glm::vec3(x, y, z), glm::vec3(sx, sy, sz))
{
}

////////////////////////////////////////////////////////////////////////////////
bool Entity::PlaySound(std::string soundFile)
{
    if (!musicPlayer.openFromFile(soundFile))
        return false; // error
    musicPlayer.play();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void Entity::RotateTo(const glm::quat & newOrientation)
{
    glm::mat4 temp = static_cast<glm::mat4>(glm::quat(newOrientation));
    temp[3] = toWorld[3];
    toWorld = std::move(temp);
    // Add scale
    ApplyScale();
    CalculateNormalMatrix();
}

////////////////////////////////////////////////////////////////////////////////
void Entity::RotateTo(const glm::mat3 & newOrientation)
{
    glm::mat4 temp = glm::mat4(newOrientation);
    temp[3] = toWorld[3];
    toWorld = std::move(temp);
    ApplyScale();
    CalculateNormalMatrix();
}

void Entity::SetScale(glm::vec3 scale)
{
    for (int col = 0; col < 3; col++)
        for (int row = 0; row < 3; row++)
            toWorld[col][row] /= this->scale[col];
    this->scale = scale;
    ApplyScale();
}

////////////////////////////////////////////////////////////////////////////////
// Getters

////////////////////////////////////////////////////////////////////////////////
glm::quat Entity::Orientation() const
{
    //return static_cast<glm::quat>(toWorld);
    // TODO: make more efficient
    glm::mat3 temp = glm::mat3(toWorld);
    for (int col = 0; col < 3; col++)
        for (int row = 0; row < 3; row++)
            temp[col][row] /= scale[col];
    return static_cast<glm::quat>(temp);
}

////////////////////////////////////////////////////////////////////////////////
void Entity::ApplyScale()
{
    if (scale != glm::vec3(1.0f))
    {
        for (int col = 0; col < 3; col++)
            for (int row = 0; row < 3; row++)
                toWorld[col][row] *= this->scale[col];
    }
}

////////////////////////////////////////////////////////////////////////////////
void Entity::CalculateNormalMatrix()
{
    glm::mat4 copy = toWorld;

    if (scale != glm::vec3(1.0f))
    {
        for (int col = 0; col < 3; col++)
            for (int row = 0; row < 3; row++)
                copy[col][row] /= this->scale[col];
    }
    normalMatrix = glm::mat3(glm::transpose(glm::inverse(copy)));
}

// Process movement
void Entity::ProcessKeyboard(POSITION position, GLfloat deltaTime)
{
    // Update the translation component of the world matrix
    if (position == P_FORWARD)
        this->toWorld[3] += 0.1f * toWorld[2];
    if (position == P_BACKWARD)
        this->toWorld[3] -= 0.1f * toWorld[2];
    if (position == P_LEFT)
        this->toWorld[3] += 0.1f * toWorld[0];
    if (position == P_RIGHT)
        this->toWorld[3] -= 0.1f * toWorld[0];
    if (position == P_UP)
        this->toWorld[3] += 0.1f * toWorld[1];
    if (position == P_DOWN)
        this->toWorld[3] -= 0.1f * toWorld[1];
}
