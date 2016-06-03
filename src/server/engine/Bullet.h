#pragma once

#include "Entity.h"
#include "BulletCollisionHandler.h"
#include "../../network/GameData.h"

class Bullet : public Entity
{
private:
	int playerId;
	int teamId;
	int damage;
	BulletCollisionHandler* c_handler;

public:

	Bullet(unsigned int objectid, int playerid, int teamid, int damage, BulletCollisionHandler* handler, btRigidBody* bullet_body, btDiscreteDynamicsWorld* physicsWorld);

	virtual ~Bullet();

	btVector3 GetBulletPosition();

	// the tick that the collision occured, may consider the object it collided with, returns true if the bullet needs to be deleted, the handler decides this
	// collidee will be nullptr if the collidee is a static object
	bool handleBulletCollision(unsigned int world_tick, Entity* collidee) { return c_handler->HandleBulletCollision(world_tick, collidee); }

	// get player id
	int GetPlayerId();

	// get team id
	int GetTeamId();

	int GetDamage();
};