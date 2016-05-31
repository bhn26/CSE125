#pragma once

#pragma once

#include "Entity.h"
#include "../../network/GameData.h"


class Grenade : public Entity
{
private:
	int playerId;
	int teamId;
	int damage;
	Entity* owner;

public:

	Grenade(int objectid, int playerid, int teamid, int damage, btVector3* pos, btVector3* velocity, btMatrix3x3* rotation, btDiscreteDynamicsWorld* physicsWorld, Entity* owner);

	virtual ~Grenade();

	btVector3 GetGrenadePosition();

	//maybe handleBulletCollision(obA, obB);

	// get player id
	int GetPlayerId();

	// get team id
	int GetTeamId();

	void HandleExplosion();
};