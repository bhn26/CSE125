#include "ConfigManager.h"

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef EGGS_CLIENT
#include "Graphics/ShaderManager.h"
#include "Graphics/ModelManager.h"
#endif

// use this to load the file into the manager
void ConfigManager::LoadConfigs(const std::string& file_name)
{
    std::string line;

    std::ifstream infile(file_name);

    while (getline(infile, line))
    {
        std::istringstream iss(line);
        std::string field, equal, value;
        if (!(iss >> field >> equal >> value)) { continue; } // error, skip this line
        if (field.substr(0, 2) == "//") { continue; }   // It's a comment
        cfg_map.insert(std::pair<std::string, std::string>(field, value));

#ifdef EGGS_CLIENT
        // Tell the ShaderManager what shaders to load
        static const std::string shaderPrefix = "Shader_";
        static const std::string modelPrefix = "Model_";
        if (field.substr(0, shaderPrefix.length()) == shaderPrefix)
        {
            // Subtract 7 for "Shader_" and 5 for "_Frag"/"_Vert"/"_Geom"
            ShaderManager::Instance()->AddShaderToLoad(field.substr(shaderPrefix.length(),
                                                                    field.length()-(shaderPrefix.length()+5)));
        }
        else if (field.substr(0, modelPrefix.length()) == modelPrefix)
        {
            std::string name = field.substr(modelPrefix.length(), field.length());
            if (name == std::string("Pumpkin")) continue;
            ModelManager::Instance()->AddModelToLoad(name);
        }
#endif
    }
    infile.close();
}

// use this to get the value read from the config file

std::string ConfigManager::GetConfigValue(const std::string& key) const
{
    if (cfg_map.find(key) != cfg_map.end())
        return cfg_map.at(key);
    else
        return "";
}

