//
//  Entity.cpp
//  egg scramble
//
//  Created by Phoebe on 4/14/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Entity.h"

bool Entity::PlaySound(std::string soundFile)
{
    if (!musicPlayer.openFromFile(soundFile))
        return false; // error
    musicPlayer.play();
}
