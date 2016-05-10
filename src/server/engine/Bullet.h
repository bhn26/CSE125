#pragma once


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

public:

	Bullet(int objectid, int playerid, int teamid);

	~Bullet();

	btVector3 GetBulletPosition();

	//maybe handleBulletCollision(obA, obB);

	// get bullet object id
	int GetObjectId();

	// get player id
	int GetPlayerId();

	// get team id
	int GetTeamId();
};