#include "StaticObject.h"
#include "../Scene.h"
#include "../Lights.h"
#include "../Camera.h"
#include "../Model.h"
#include "../Shader.h"

#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

StaticObject::StaticObject(std::shared_ptr<Model> model) : Entity(), m_model(std::move(model))
{
    m_shader = ShaderManager::Instance()->GetShader("Model");
}

StaticObject::~StaticObject()
{
    // delete model;
}

void StaticObject::Translate(const glm::vec3& translate)
{
    // m_toWorld = glm::translate(glm::mat4(1.0f), translate) * m_toWorld;
    m_toWorld[3] = std::move(glm::vec4(translate, 1.0f));
}

void StaticObject::Rotate(float deg, const glm::vec3& axis)
{
    // m_toWorld = glm::rotate(glm::mat4(1.0f), glm::radians(deg), axis) * m_toWorld;
    // this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(m_toWorld)));
    RotateTo(glm::mat3(glm::rotate(m_toWorld, glm::radians(deg), axis)));
}

void StaticObject::Draw() const
{
    // Use the appropriate m_shader (depth or model)
    UseShader();

    // Draw the loaded model
    m_model->Draw(Scene::Instance()->IsRenderingDepth() ? nullptr : m_shader.get());
}

void StaticObject::Update(float deltaTime)
{
    // Spin(0.3f);
}

void StaticObject::Spin(float deg)
{
    // This creates the matrix to rotate the cube
    m_toWorld =
        m_toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    CalculateNormalMatrix();
}

void StaticObject::SetShaderUniforms() const
{
    glUniform1i(m_shader->GetUniform("shadowMap"), Scene::Instance()->ShadowMapIndex());
    glActiveTexture(GL_TEXTURE0 + Scene::Instance()->ShadowMapIndex());
    glBindTexture(GL_TEXTURE_2D, Scene::Instance()->DepthMap());

    glUniformMatrix4fv(m_shader->GetUniform("model"), 1, false, glm::value_ptr(m_toWorld));
    glUniformMatrix3fv(
        m_shader->GetUniform("normalMatrix"), 1, false, glm::value_ptr(m_normalMatrix));
    glUniformMatrix4fv(
        m_shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(m_shader->GetUniform("projection"),
                       1,
                       false,
                       glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
    glUniformMatrix4fv(m_shader->GetUniform("lightSpaceMatrix"),
                       1,
                       false,
                       glm::value_ptr(Scene::Instance()->LightSpaceMatrix()));
    LoadDirectionalLight(Scene::Instance()->GetDirectionalLight());
}
