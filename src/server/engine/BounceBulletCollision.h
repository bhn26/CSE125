#pragma once
#include "BulletCollisionHandler.h"
#include "Bullet.h"

// simple collision handler, always signals the bullet for deletion on collision
class BounceBulletCollision : public BulletCollisionHandler
{
public:
	// the number of bounces the bullet should have
	BounceBulletCollision(int b, btVector3* initial_speed, btMatrix3x3* rot) {
		bounces = b;  
		bullet = nullptr;
		speed = initial_speed;
	};
	~BounceBulletCollision() { delete speed; };

	bool HandleBulletCollision(unsigned int world_tick) {
		printf("collide %d\n", bounces);
		if (bullet == nullptr)
			return false;

		if (--bounces == 0)
			return true;
		bullet->GetRigidBody()->setLinearVelocity(*speed);

		return false;
	}

	// This needs to be called after the constructor because hack.
	void SetBullet(Bullet* bul)
	{
		bullet = bul;
	}

private:
	int bounces; //the number of bounces left before the bullet should expire
	Bullet* bullet; // the bullet that owns this collision handler
	btVector3* speed; // initial speed of the bullet
};