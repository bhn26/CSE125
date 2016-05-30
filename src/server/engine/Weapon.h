#pragma once

#include "../../network/GameData.h"
//#include <vector>
//#include <memory>
#include "FireRateReset.h"
#include "FieldHandler.h"

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

class Entity;

class Weapon
{

protected:
	int fireRate;
	int damage;
	btDiscreteDynamicsWorld* curWorld;

public:

	unsigned int nextFireTick;
	int fireFlag;

	Weapon(int firerate, int wdamage, btDiscreteDynamicsWorld* curworld);
	~Weapon();

	void virtual UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner);

	void virtual ReloadWeapon();
};