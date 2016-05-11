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

using namespace std;

enum ConfigField {
    IP,
    PORT
};

class ConfigLoader {
public:
    map<int, string> cfg_map;
    
    ConfigLoader(){};
    
    void LoadConfigs(string fp) {
        string line;
        
        ifstream infile("eggs.cfg");
        
        int n = 0; // config field
        while (getline(infile, line))
        {
            istringstream iss(line);
            string field, equal, value;
            if (!(iss >> field >> equal >> value)) { break; } // error
            
            cfg_map[n] = value;
        }
    }
    
};

#endif /* ConfigLoader_h */
