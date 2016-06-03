#pragma once

// Different types of bullet collision handling
class BulletCollisionHandler {

public:
	// returns true when the bullet should be deleted
	virtual bool HandleBulletCollision(unsigned int world_tick) { return true; }

};