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

public:

	unsigned int nextFireTick;
	int fireFlag;

	Weapon(int firerate, int wdamage, std::vector<std::shared_ptr<Weapon>>* frreset);
	~Weapon();

	void virtual UseWeapon();

	void virtual ReloadWeapon();
};