#include "ShaderManager.h"
#include "Shader.h"
#include "ConfigManager.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
const std::string ShaderManager::shaderPath = "src/Graphics/Shaders/";

////////////////////////////////////////////////////////////////////////////////
void ShaderManager::LoadShaders()
{
    printf("\n=== Loading Shaders ===\n");
    for (std::string& shaderName : _shaderNames)
    {
        LoadShader(shaderName);
    }
    _shaderNames.clear();
}

////////////////////////////////////////////////////////////////////////////////
bool ShaderManager::LoadShader(const std::string& shaderName)
{
    printf("Attempting to load %s...\t", shaderName.c_str());
    // Don't duplicate load
    if (_shaderMap.find(shaderName) != _shaderMap.end())
    {
        printf("Duplicate found!\n");
        return false;
    }

    const static std::string shaderPrefix = std::string("Shader_");
    const static std::string vertSuffix = std::string("_Vert");
    const static std::string fragSuffix = std::string("_Frag");
    std::string vertShaderPath = ConfigManager::Instance()->GetConfigValue(shaderPrefix + shaderName + vertSuffix);
    std::string fragShaderPath = ConfigManager::Instance()->GetConfigValue(shaderPrefix + shaderName + fragSuffix);
    if (!vertShaderPath.length() || !fragShaderPath.length())   // Make sure we get the shader paths
    {
        printf("Error: No vertex/fragment shader pair in config files!\n");
        return false;
    }

    //_shaderMap[shaderName] = std::make_shared<Shader>(shaderPath + vertShaderPath, shaderPath + fragShaderPath);
    _shaderMap[shaderName] = std::shared_ptr<Shader>(new Shader(shaderPath + vertShaderPath, shaderPath + fragShaderPath));
    printf("Done!\n");
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void ShaderManager::AddShaderToLoad(std::string shaderName)
{
    _shaderNames.push_back(shaderName);
}

////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Shader> ShaderManager::GetShader(std::string shaderName)
{
    const ShaderManager* manager = Instance();
    std::map<std::string, std::shared_ptr<Shader>>::const_iterator it = manager->_shaderMap.find(shaderName);
    // If in map
    if (it != manager->_shaderMap.end())    // Get shader mapped to this string
    {
        return it->second;
    }
    else    // Print error message
    {
        std::cout << "Shader not found in map\n";
        return std::shared_ptr<Shader>(nullptr);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ShaderManager::ApplyUboToAllShaders(std::string blockName, int binding)
{
    for (auto& shaderPair : _shaderMap)
    {
        GLuint program = shaderPair.second->GetProgram();
        int blockIndex = glGetUniformBlockIndex(program, blockName.c_str());
        if (blockIndex != GL_INVALID_INDEX)
            glUniformBlockBinding(program, blockIndex, binding);
    }
}
