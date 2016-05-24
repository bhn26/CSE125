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
    return true;
}


// Process movement
void Entity::ProcessKeyboard(POSITION position, GLfloat deltaTime)
{
	// Update the translation component of the world matrix
	if (position == P_FORWARD)
		this->toWorld[3] += deltaTime * toWorld[2];
	if (position == P_BACKWARD)
		this->toWorld[3] -= deltaTime * toWorld[2];
	if (position == P_LEFT)
		this->toWorld[3] += deltaTime * toWorld[0];
	if (position == P_RIGHT)
		this->toWorld[3] -= deltaTime * toWorld[0];
	if (position == P_UP)
		this->toWorld[3] += deltaTime * toWorld[1];
	if (position == P_DOWN)
		this->toWorld[3] -= deltaTime * toWorld[1];
}
