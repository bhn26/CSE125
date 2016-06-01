#pragma once
#include "BulletCollisionHandler.h"
#include <stdio.h>	

// simple 
class SimpleBulletCollision : public BulletCollisionHandler
{
public:
	SimpleBulletCollision() {};
	~SimpleBulletCollision() {};

	bool HandleBulletCollision(unsigned int world_tick) {
		printf("simple bullet collider\n");
		return true;
	}
};
