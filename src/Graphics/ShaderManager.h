#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <vector>
#include <string>

#include "ConfigManager.h"

class Shader;

class ShaderManager
{
    std::map<std::string, std::shared_ptr<Shader>> _shaderMap;
    std::vector<std::string> _shaderNames;

    ShaderManager() {}
    bool LoadShader(const std::string& shaderName);

public:
    const static std::string shaderPath;
    void AddShaderToLoad(std::string shaderName);
    void LoadShaders();
    static ShaderManager* Instance()
    {
        static ShaderManager* instance = new ShaderManager();
        return instance;
    }

    static std::shared_ptr<Shader> GetShader(std::string shaderName);

    void ApplyUBOToAllShaders(std::string blockName, int index);
};
