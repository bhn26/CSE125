//
//  Entity.hpp
//  egg scramble
//
//  Created by Phoebe on 4/14/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#include <string>
#include <SFML/Audio.hpp>
#include "../Animation/ogldev_util.h"

class Entity
{
    long long m_startTime;
    sf::Music musicPlayer;

public:
    Entity() {}

    bool PlaySound(std::string soundFile);
    float GetRunningTime()
    {
        return (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;
    }

    // Draw
    // Update
};


#endif /* Entity_hpp */
