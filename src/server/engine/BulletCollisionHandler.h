#pragma once

// Different types of bullet collision handling
class BulletCollisionHandler {

public:
	virtual bool HandleBulletCollision(unsigned int world_tick) { return true; }

};