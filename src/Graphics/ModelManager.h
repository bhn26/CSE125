#pragma once
#include "Basic/Singleton.h"

#include <memory>
#include <map>
#include <vector>

class Model;

// ModelManager should be used to use any model.
// ModelManager owns all models and allows others to use them as well.
// ModelManager gets all models via config files in the form of Model_<name>
class ModelManager : public Singleton<ModelManager>
{
    friend class Singleton<ModelManager>;

    std::map<std::string, std::shared_ptr<Model>> m_modelMap;
    std::vector<std::string> m_modelNames;

    ModelManager() {}
    bool LoadModel(const std::string& modelName);

public:
    void AddModelToLoad(std::string modelName);
    void LoadModels();

    static std::shared_ptr<Model> GetModel(std::string modelName);
};
