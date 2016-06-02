#pragma once
#include "BulletCollisionHandler.h"

// simple collision handler, always signals the bullet for deletion on collision
class SimpleBulletCollision : public BulletCollisionHandler
{
public:
	SimpleBulletCollision() {};
	~SimpleBulletCollision() {};

	bool HandleBulletCollision(unsigned int world_tick) {
		return true;
	}
};
