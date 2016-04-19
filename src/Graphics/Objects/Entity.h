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

class Entity
{
    sf::Music musicPlayer;

public:
    Entity() {}

    bool PlaySound(std::string soundFile);
};


#endif /* Entity_hpp */
