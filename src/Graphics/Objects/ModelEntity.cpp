#include "ModelEntity.h"
#include "../Model.h"
#include "../ShaderManager.h"

ModelEntity::ModelEntity(std::shared_ptr<Model> model) : Entity(), m_model(model)
{
    this->shader = ShaderManager::Instance()->GetShader("Model");
}

void ModelEntity::Draw() const
{
    m_model->Draw(this->shader.get());
}
