//
//  Entity.cpp
//  egg scramble
//
//  Created by Phoebe on 4/14/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Entity.h"
#include "Graphics/Shader.h"
#include "Graphics/Lights.h"
#include "Graphics/Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

////////////////////////////////////////////////////////////////////////////////
// NOTE: Constructors do not initialize vertex/element buffers, nor m_shader
Entity::Entity(int oid, int cid) : Entity()
{
    m_objId = oid;
    m_classId = cid;
}

////////////////////////////////////////////////////////////////////////////////
Entity::Entity(glm::mat4 world, glm::vec3 m_scale)
    : m_toWorld(world), m_scale(m_scale), m_VBO(0), m_VAO(0), m_EBO(0), m_shader(nullptr)
{
    ApplyScale();
    CalculateNormalMatrix();
}

////////////////////////////////////////////////////////////////////////////////
Entity::Entity(glm::vec3 pos, glm::vec3 m_scale)
    : Entity(glm::translate(glm::mat4(1.0f), pos), m_scale)
{
}

////////////////////////////////////////////////////////////////////////////////
Entity::Entity(float x, float y, float z, float sx, float sy, float sz)
    : Entity(glm::vec3(x, y, z), glm::vec3(sx, sy, sz))
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
void Entity::RotateTo(const glm::quat& newOrientation)
{
    glm::mat4 temp = static_cast<glm::mat4>(glm::quat(newOrientation));
    temp[3] = m_toWorld[3];
    m_toWorld = std::move(temp);
    // Add m_scale
    ApplyScale();
    CalculateNormalMatrix();
}

////////////////////////////////////////////////////////////////////////////////
void Entity::RotateTo(const glm::mat3& newOrientation)
{
    glm::mat4 temp = glm::mat4(newOrientation);
    temp[3] = m_toWorld[3];
    m_toWorld = std::move(temp);
    ApplyScale();
    CalculateNormalMatrix();
}

void Entity::SetScale(const glm::vec3& scale)
{
    for (int col = 0; col < 3; col++)
        for (int row = 0; row < 3; row++)
            m_toWorld[col][row] /= scale[col];
    m_scale = scale;
    ApplyScale();
}

void Entity::UseShader() const
{
    // For rendering Depth, only need model
    if (Scene::Instance()->IsRenderingDepth())
    {
        std::shared_ptr<Shader>& m_shader = Scene::Instance()->GetDepthShader();
        m_shader->Use();
        glUniformMatrix4fv(m_shader->GetUniform("model"), 1, GL_FALSE, glm::value_ptr(m_toWorld));
    }
    else
    {
        m_shader->Use();
        this->SetShaderUniforms();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Getters

////////////////////////////////////////////////////////////////////////////////
glm::quat Entity::Orientation() const
{
    // return static_cast<glm::quat>(m_toWorld);
    // TODO: make more efficient
    glm::mat3 temp = glm::mat3(m_toWorld);
    for (int col = 0; col < 3; col++)
        for (int row = 0; row < 3; row++)
            temp[col][row] /= m_scale[col];
    return static_cast<glm::quat>(temp);
}

////////////////////////////////////////////////////////////////////////////////
void Entity::ApplyScale()
{
    if (m_scale != glm::vec3(1.0f))
    {
        for (int col = 0; col < 3; col++)
            for (int row = 0; row < 3; row++)
                m_toWorld[col][row] *= m_scale[col];
    }
}

////////////////////////////////////////////////////////////////////////////////
void Entity::CalculateNormalMatrix()
{
    glm::mat4 copy = m_toWorld;

    if (m_scale != glm::vec3(1.0f))
    {
        for (int col = 0; col < 3; col++)
            for (int row = 0; row < 3; row++)
                copy[col][row] /= m_scale[col];
    }
    m_normalMatrix = glm::mat3(glm::transpose(glm::inverse(copy)));
}

void Entity::LoadDirectionalLight(DirectionalLight* dLight) const
{
    glUniform3fv(m_shader->GetUniform("dLight._base._color"), 1, glm::value_ptr(dLight->m_color));
    glUniform1f(m_shader->GetUniform("dLight._base._ambientIntensity"), dLight->m_ambientIntensity);
    glUniform1f(m_shader->GetUniform("dLight._base._diffuseIntensity"), dLight->m_diffuseIntensity);
    glUniform3fv(m_shader->GetUniform("dLight._direction"), 1, glm::value_ptr(dLight->m_direction));
}

// Process movement
void Entity::ProcessKeyboard(PositionKey position, GLfloat deltaTime)
{
    // Update the translation component of the world matrix
    if (position == PositionKey::Forward)
        m_toWorld[3] += 0.1f * m_toWorld[2];
    if (position == PositionKey::Backward)
        m_toWorld[3] -= 0.1f * m_toWorld[2];
    if (position == PositionKey::Left)
        m_toWorld[3] += 0.1f * m_toWorld[0];
    if (position == PositionKey::Right)
        m_toWorld[3] -= 0.1f * m_toWorld[0];
    if (position == PositionKey::Up)
        m_toWorld[3] += 0.1f * m_toWorld[1];
    if (position == PositionKey::Down)
        m_toWorld[3] -= 0.1f * m_toWorld[1];
}
