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

	// the tick that the collision occured, returns true if the bullet needs to be deleted, the handler decides this
	bool handleBulletCollision(unsigned int world_tick) { return c_handler->HandleBulletCollision(world_tick); }

	// get player id
	int GetPlayerId();

	// get team id
	int GetTeamId();

	int GetDamage();
};