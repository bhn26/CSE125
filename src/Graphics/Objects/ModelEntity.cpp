#include "ModelEntity.h"
#include "Graphics/Model.h"
#include "Graphics/Scene.h"
#include "Graphics/ShaderManager.h"

#include <glm/gtc/type_ptr.hpp>

ModelEntity::ModelEntity(std::shared_ptr<Model> model) : Entity(), m_model(std::move(model))
{
    this->m_shader = ShaderManager::GetShader("Model");
}

void ModelEntity::Draw() const
{
    // Use the appropriate m_shader (depth or model)
    UseShader();

    // Draw the loaded model
    m_model->Draw(Scene::Instance()->IsRenderingDepth() ? nullptr : m_shader.get());
}

void ModelEntity::SetShaderUniforms() const
{
    glUniformMatrix4fv(
        m_shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(m_shader->GetUniform("model"), 1, false, glm::value_ptr(m_toWorld));
    glUniformMatrix4fv(m_shader->GetUniform("projection"),
                       1,
                       false,
                       glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
}
