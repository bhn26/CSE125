#pragma once

#include "Entity.h"
#include "../Model.h"
#include <memory>
#include <string>

//class Model;

class ModelEntity : public Entity
{
public:
    ModelEntity(std::shared_ptr<Model> model);

private:
    std::shared_ptr<Model> m_model;

    // Inherited via Entity
    virtual void Draw() const override;
    virtual void SetShaderUniforms() const override;
};