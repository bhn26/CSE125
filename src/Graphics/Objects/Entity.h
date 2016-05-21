//
//  Entity.hpp
//  egg scramble
//
//  Created by Phoebe on 4/14/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <SFML/Audio.hpp>

#include "Basic/Utils.h"
#include "../Shader.h"

class Entity
{
    long long m_startTime;
    sf::Music musicPlayer;
protected:
    glm::mat4 toWorld;
    glm::mat3 normalMatrix;
    GLuint VBO, VAO, EBO;
    std::shared_ptr<Shader> shader;
	int obj_id; // This refers to the specific object
	int class_id; // This refers to the type of object

public:
    // NOTE: Constructors do not initialize vertex/element buffers, nor shader
	Entity(int oid, int cid) : Entity()
	{
		obj_id = oid;
		class_id = cid;
	}

    Entity(glm::mat4 world = glm::mat4(1.0f)) : toWorld(world), VBO(0), VAO(0), EBO(0), shader(nullptr)
    {
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(world)));
    }

    Entity(glm::vec3 pos) : Entity(glm::translate(glm::mat4(1.0f), pos))
    {
    }

    Entity(float x, float y, float z) : Entity(glm::vec3(x, y, z))
    {
    }

    bool PlaySound(std::string soundFile);
    float GetRunningTime()
    {
        return (float)(Utils::CurrentTime() - m_startTime);
    }

    virtual void Draw() const = 0;
    virtual void Update() = 0;

    virtual void Spawn(/*Scene* scene, */float x, float y, float z) = 0;      // Maybe to spawn into the world, rather than using a constructor

    virtual void MoveTo(float x, float y, float z) { MoveTo(glm::vec3(x, y, z)); }
    virtual void MoveTo(const glm::vec3& newPosition) { toWorld[3] = glm::vec4(newPosition, 1.0f); }
    virtual void RotateTo(float w, float x, float y, float z) { RotateTo(glm::quat(w, x, y, z)); }
    virtual void RotateTo(const glm::quat& newOrientation) { toWorld = glm::translate(static_cast<glm::mat4>(glm::quat(newOrientation)), glm::vec3(toWorld[3])); }
    virtual void RotateTo(const glm::mat3& newOrientation) { toWorld = glm::translate(glm::mat4(newOrientation), glm::vec3(toWorld[3])); }

    glm::quat Orientation() const { return static_cast<glm::quat>(toWorld); }

    const glm::mat4& ToWorld() const { return toWorld; }
    const glm::mat3& NormalMatrix() const { return normalMatrix; }
    //const std::shared_ptr<Shader>& Shader() const { return shader; }      // Creates error with typename Shader
    const glm::vec3& Position() const { return glm::vec3(toWorld[3]); }

    std::shared_ptr<Shader>& GetShader() { return shader; }

private:

};
