#pragma once
#include "BulletCollisionHandler.h"
#include "Bullet.h"
// Explosive handler that explodes on contact
class ExplosiveBulletCollision : public BulletCollisionHandler
{
public:
	// the number of bounces the bullet should have
	ExplosiveBulletCollision(int size, int ttl) {
		this->ttl = ttl;
		explosion_size = size;
		bullet = nullptr;
	};

	~ExplosiveBulletCollision() {};

	// This needs to be called after the constructor because hack.
	void SetBullet(Bullet* bul)
	{
		bullet = bul;
	}

	bool HandleBulletCollision(unsigned int world_tick);

private:
	int explosion_size; // size of the explosion
	int ttl; // duration of the explosion
	Bullet* bullet; // the bullet that owns this collision handler
};