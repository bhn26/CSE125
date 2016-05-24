#include "ModelEntity.h"
#include "../Model.h"

ModelEntity::ModelEntity(std::string filename) : Entity()
{
    m_model = std::unique_ptr<Model>(new Model(filename.c_str()));
    this->shader = std::make_shared<Shader>("src/Graphics/Shaders/model_loading.vert", "src/Graphics/Shaders/model_loading.frag");
}

void ModelEntity::Draw() const
{
    m_model->Draw(this->shader.get());
}
