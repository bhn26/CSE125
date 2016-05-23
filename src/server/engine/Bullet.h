#pragma once

#include "../../network/GameData.h"
#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

class Bullet
{
private:
	int id;
	int playerId;
	int teamId;
	int damage;
	btDiscreteDynamicsWorld* curWorld;
	btRigidBody* bulletRigidBody;

public:

	Bullet(int objectid, int playerid, int teamid, int damage, PosInfo pos, btVector3* velocity, btDiscreteDynamicsWorld* physicsWorld);

	~Bullet();

	btVector3 GetBulletPosition();

	//maybe handleBulletCollision(obA, obB);

	// get bullet object id
	int GetObjectId();

	// get player id
	int GetPlayerId();

	// get team id
	int GetTeamId();

	int GetDamage();
};