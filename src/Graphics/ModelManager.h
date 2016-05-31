#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <vector>

class Model;

class ModelManager
{
    std::map<std::string, std::shared_ptr<Model>> _modelMap;
    std::vector<std::string> _modelNames;

    ModelManager() {}
    bool LoadModel(const std::string& shaderName);

public:
    void AddModelToLoad(std::string modelName);
    void LoadModels();
    static ModelManager* Instance()
    {
        static ModelManager* instance = new ModelManager();
        return instance;
    }

    static std::shared_ptr<Model> GetModel(std::string shaderName);
};
