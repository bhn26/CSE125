#include "ShaderManager.h"
#include "iostream"
#include "Shader.h"
#include "ConfigManager.h"

////////////////////////////////////////////////////////////////////////////////
const std::string ShaderManager::shaderPath = "src/Graphics/Shaders/";

////////////////////////////////////////////////////////////////////////////////
void ShaderManager::LoadShaders()
{
    for (std::string& shaderName : _shaderNames)
    {
        LoadShader(shaderName);
    }
    _shaderNames.clear();
}

////////////////////////////////////////////////////////////////////////////////
bool ShaderManager::LoadShader(const std::string& shaderName)
{
    // Don't duplicate load
    if (_shaderMap.find(shaderName) != _shaderMap.end())
    {
        return false;
    }

    const static std::string shaderPrefix = std::string("Shader_");
    const static std::string vertSuffix = std::string("_Vert");
    const static std::string fragSuffix = std::string("_Frag");
    std::string vertShaderPath = ConfigManager::instance()->GetConfigValue(shaderPrefix + shaderName + vertSuffix);
    std::string fragShaderPath = ConfigManager::instance()->GetConfigValue(shaderPrefix + shaderName + fragSuffix);
    if (!vertShaderPath.length() || !fragShaderPath.length())   // Make sure we get the shader paths
    {
        return false;
    }

    //_shaderMap[shaderName] = std::make_shared<Shader>(shaderPath + vertShaderPath, shaderPath + fragShaderPath);
    _shaderMap[shaderName] = std::shared_ptr<Shader>(new Shader(shaderPath + vertShaderPath, shaderPath + fragShaderPath));
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
    ShaderManager* manager = Instance();
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
void ShaderManager::ApplyUBOToAllShaders(std::string blockName, int index)
{
    for (auto& shaderPair : _shaderMap)
    {
        GLuint program = shaderPair.second->GetProgram();
        int blockIndex = glGetUniformBlockIndex(program, blockName.c_str());
        if (blockIndex != GL_INVALID_INDEX)
            glUniformBlockBinding(program, blockIndex, index);
    }
}
