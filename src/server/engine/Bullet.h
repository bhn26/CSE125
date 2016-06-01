#pragma once

#include "Entity.h"
#include "Player.h"
#include "Weapon.h"
#include "../../network/GameData.h"

class Bullet : public Entity
{
private:
	int playerId;
	int teamId;
	int damage;

public:

	Bullet(unsigned int objectid, int playerid, int teamid, int damage, btRigidBody* bullet_body, btDiscreteDynamicsWorld* physicsWorld);

	virtual ~Bullet();

	btVector3 GetBulletPosition();

	//maybe handleBulletCollision(obA, obB);

	// get player id
	int GetPlayerId();

	// get team id
	int GetTeamId();

	int GetDamage();
};