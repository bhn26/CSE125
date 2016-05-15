#pragma once

#include "../../network/GameData.h"
#include <vector>
#include <memory>

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

class Weapon
{

protected:
	int fireRate;
	int damage;
	std::vector<std::shared_ptr<Weapon>>* frReset;
	btDiscreteDynamicsWorld* curWorld;

public:

	unsigned int nextFireTick;
	int fireFlag;

	Weapon(int firerate, int wdamage, std::vector<std::shared_ptr<Weapon>>* frreset, btDiscreteDynamicsWorld* curworld);
	~Weapon();

	void virtual UseWeapon(const btVector3* position, btMatrix3x3* rotation, int playerid, int teamid);

	void virtual ReloadWeapon();
};