#pragma once

#include "../../network/GameData.h"
#include <vector>
#include <memory>
#include "FireRateReset.h"

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
	int currentAmmo; // Current ammo of this weapon
	int maxAmmo;   // Capacity of this weapon
	btVector3* gunSpeed;

	WeaponType wt;

	btDiscreteDynamicsWorld* curWorld;

public:

	unsigned int nextFireTick;
	int fireFlag;

	Weapon(int firerate, int wdamage, btDiscreteDynamicsWorld* curworld);
	~Weapon();

	// returns the ammo left in the weapon
	int virtual UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner);

	WeaponType virtual GetWeaponType() { return wt; }

	btVector3* GetGunSpeed() { return gunSpeed; }

	void virtual ReloadWeapon();
};