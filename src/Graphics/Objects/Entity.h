#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "../Shader.h"

class Entity
{
protected:
    glm::mat4 toWorld;
    glm::mat3 normalMatrix;
    GLuint VBO, VAO, EBO;
    std::shared_ptr<Shader> shader;

public:
    // NOTE: Constructors do not initialize vertex/element buffers, nor shader
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

    virtual void Draw() const = 0;
    virtual void Update() = 0;

    virtual void Spawn(/*Scene* scene, */float x, float y, float z) = 0;      // Maybe to spawn into the world, rather than using a constructor

    const glm::mat4& ToWorld() const { return toWorld; }
    const glm::mat3& NormalMatrix() const { return normalMatrix; }
    //const std::shared_ptr<Shader>& Shader() const { return shader; }      // Creates error with typename Shader
    glm::vec3 Position() { return glm::vec3(toWorld[3]); }

    std::shared_ptr<Shader>& GetShader() { return shader; }

private:

};