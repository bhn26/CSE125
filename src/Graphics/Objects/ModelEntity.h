#pragma once

#include "Entity.h"
#include "../Model.h"
#include <memory>
#include <string>

//class Model;

class ModelEntity : public Entity
{
public:
    ModelEntity(std::string filename = "");

private:
    std::unique_ptr<Model> m_model;

    // Inherited via Entity
    virtual void Draw() const override;
    virtual void Update() override;
    virtual void Spawn(float x, float y, float z) override;
};