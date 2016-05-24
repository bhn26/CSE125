#include "ShaderManager.h"
#include "iostream"
#include "Shader.h"

void ShaderManager::Initialize()
{
    _shaderMap["Instance"] = std::make_shared<Shader>("src/Graphics/Shaders/instancing.vert", "src/Graphics/Shaders/instancing.frag");
    _shaderMap["Basic"] = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/basic_shader.frag");
    _shaderMap["Diffuse"] = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/diffuse.frag");
    _shaderMap["Model"] = std::make_shared<Shader>("src/Graphics/Shaders/model_loading.vert", "src/Graphics/Shaders/model_loading.frag");
    _shaderMap["CubeMap"] = std::make_shared<Shader>("src/Graphics/Shaders/cubemap.vert", "src/Graphics/Shaders/cubemap.frag");
}

std::shared_ptr<Shader> ShaderManager::GetShader(std::string shaderName)
{
    std::string vert = ConfigManager::instance()->GetConfigValue("Shader_" + shaderName + "_Vert");
    std::string frag = ConfigManager::instance()->GetConfigValue("Shader_" + shaderName + "_Frag");

    std::map<std::string, std::shared_ptr<Shader>>::iterator it = _shaderMap.find(shaderName);
    // If in map
    if (it != _shaderMap.end())    // Get shader mapped to this string
    {
        return it->second;
    }
    else    // Print error message
    {
        std::cout << "Shader not found in map\n";
        return std::shared_ptr<Shader>(nullptr);
    }
}