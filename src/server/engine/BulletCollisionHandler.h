#pragma once
#include "Entity.h"

// Different types of bullet collision handling
class BulletCollisionHandler {

public:
	// returns true when the bullet should be deleted
	// world tick that it collided on, collidee is the thing that it collided with. Will be nullptr if it's a static object
	virtual bool HandleBulletCollision(unsigned int world_tick, Entity* collidee) { return true; }

};