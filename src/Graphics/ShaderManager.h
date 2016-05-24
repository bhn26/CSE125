#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <map>

#include "ConfigManager.h"

class Shader;

class ShaderManager
{
    std::map<std::string, std::shared_ptr<Shader>> _shaderMap;

    ShaderManager() {}

public:
    void Initialize();
    static ShaderManager* Instance()
    {
        static ShaderManager* instance = new ShaderManager();
        return instance;
    }

    std::shared_ptr<Shader> GetShader(std::string shaderName);
};
