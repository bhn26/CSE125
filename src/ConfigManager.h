//
//  ConfigLoader.hpp
//  egg scramble
//
//  Created by Phoebe on 4/18/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef ConfigLoader_h
#define ConfigLoader_h

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

class ConfigManager
{
public:
    std::map<std::string, std::string> cfg_map;
    
    // use this to load the file into the manager
    void LoadConfigs(const std::string& file_name);

    // use this to get the value read from the config file
    std::string GetConfigValue(const std::string& key) const;

	static void instantiate()
	{
		if (cfg == nullptr)
			cfg = new ConfigManager();
	}

	static ConfigManager* instance() { return cfg; }

private:
	static ConfigManager * cfg;
    ConfigManager(void) {}
    ~ConfigManager(void) {}
};

#endif /* ConfigLoader_h */
