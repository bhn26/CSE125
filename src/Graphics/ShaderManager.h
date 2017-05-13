#pragma once
#include "Basic/Singleton.h"

#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <vector>
#include <string>

class Shader;

// ShaderManager should be used to use shaders.
// ShaderManager owns all shaders and allows others to use them as well.
// ShaderManager gets all shaders via config files in the form of Shader_<name>_Vert and Shader_<name>_Frag pairs.
// Shader_<name>_Geom should be optional and is not yet supported
class ShaderManager : public Singleton<ShaderManager>
{
    friend class Singleton<ShaderManager>;
    friend class ConfigManager;

    std::map<std::string, std::shared_ptr<Shader>> _shaderMap;
    std::vector<std::string> _shaderNames;

    ShaderManager() {}
    bool LoadShader(const std::string& shaderName);

    void AddShaderToLoad(std::string shaderName);   // Called from ConfigManager

public:
    const static std::string shaderPath;

    void LoadShaders();

    static std::shared_ptr<Shader> GetShader(std::string shaderName);

    void ApplyUboToAllShaders(std::string blockName, int binding);
};
