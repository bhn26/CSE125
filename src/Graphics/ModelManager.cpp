#include "ModelManager.h"
#include "ConfigManager.h"
#include "Model.h"

#include <iostream>

void ModelManager::LoadModels()
{
    for (std::string& shaderName : _modelNames)
    {
        LoadModel(shaderName);
    }
    _modelNames.clear();
}

bool ModelManager::LoadModel(const std::string& modelName)
{
    // Don't duplicate load
    if (_modelMap.find(modelName) != _modelMap.end())
    {
        return false;
    }

    const static std::string modelPrefix = std::string("Model_");
    std::string modelPath = ConfigManager::instance()->GetConfigValue(modelPrefix + modelName);
    if (!modelPath.length())   // Make sure we get the shader paths
    {
        return false;
    }

    Model* model = new Model(modelPath);
    _modelMap[modelName] = std::shared_ptr<Model>(model);
    return true;
}

void ModelManager::AddModelToLoad(std::string modelName)
{
    _modelNames.push_back(modelName);
}

std::shared_ptr<Model> ModelManager::GetModel(std::string modelName)
{
    const ModelManager* manager = Instance();
    std::map<std::string, std::shared_ptr<Model>>::const_iterator it = manager->_modelMap.find(modelName);
    // If in map
    if (it != manager->_modelMap.end())    // Get shader mapped to this string
    {
        return it->second;
    }
    else    // Print error message
    {
        std::cout << "Shader not found in map\n";
        return std::shared_ptr<Model>(nullptr);
    }
}