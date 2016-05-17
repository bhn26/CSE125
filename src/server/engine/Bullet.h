#pragma once

#include "Entity.h"
#include "../../network/GameData.h"


class Bullet : public Entity
{
private:
	int id;
	int playerId;
	int teamId;
	int damage;
	btDiscreteDynamicsWorld* curWorld;
	btRigidBody* bulletRigidBody;

public:

	Bullet(int objectid, int playerid, int teamid, int damage, const btVector3* pos, btVector3* velocity, btDiscreteDynamicsWorld* physicsWorld);

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