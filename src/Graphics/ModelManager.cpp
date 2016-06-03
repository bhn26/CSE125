#include "ModelManager.h"
#include "ConfigManager.h"
#include "Model.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
void ModelManager::LoadModels()
{
    printf("\n=== Loading Models ===\n");
    for (std::string& modelName : _modelNames)
    {
        if (modelName == std::string("Pumpkin_Patch"))
        {
            //printf("Pumpkin_Patch!\n");
        }
        LoadModel(modelName);
    }
    _modelNames.clear();
}

////////////////////////////////////////////////////////////////////////////////
bool ModelManager::LoadModel(const std::string& modelName)
{
    printf("Attempting to load %s...\t", modelName.c_str());
    // Don't duplicate load
    if (_modelMap.find(modelName) != _modelMap.end())
    {
        printf("Duplicate found!\n");
        return false;
    }

    const static std::string modelPrefix = std::string("Model_");
    std::string modelPath = ConfigManager::instance()->GetConfigValue(modelPrefix + modelName);
    if (!modelPath.length())   // Make sure we get the model paths
    {
        printf("Error: No model in config files!\n");
        return false;
    }

    Model* model = new Model(modelPath);
    _modelMap[modelName] = std::shared_ptr<Model>(model);
    printf("Done!\n");
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void ModelManager::AddModelToLoad(std::string modelName)
{
    _modelNames.push_back(modelName);
}

////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Model> ModelManager::GetModel(std::string modelName)
{
    const ModelManager* manager = Instance();
    std::map<std::string, std::shared_ptr<Model>>::const_iterator it = manager->_modelMap.find(modelName);
    // If in map
    if (it != manager->_modelMap.end())    // Get model mapped to this string
    {
        return it->second;
    }
    else    // Print error message
    {
        std::cout << "Shader not found in map\n";
        return std::shared_ptr<Model>(nullptr);
    }
}