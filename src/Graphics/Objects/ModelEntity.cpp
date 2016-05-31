#include "ModelEntity.h"
#include "../Model.h"
#include "../Scene.h"
#include "Graphics/ShaderManager.h"

#include <glm/gtc/type_ptr.hpp>

ModelEntity::ModelEntity(std::string filename) : Entity()
{
    m_model = std::unique_ptr<Model>(new Model(filename.c_str()));
    this->shader = ShaderManager::GetShader("Model");
}

void ModelEntity::Draw() const
{
    // Use the appropriate shader (depth or model)
    UseShader();

    // Draw the loaded model
    m_model->Draw(this->shader.get());
}

void ModelEntity::SetShaderUniforms() const
{
    glUniformMatrix4fv(shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix4fv(shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
}
