#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class SeedGun : public Weapon
{
protected:
	int gunfireRate = 1;
	int gunDamage = 100;
	btVector3* gunSpeed = new btVector3(0, 8, 25);

	// resets the reloaded flag when tick reaches nextFireTick

public:
	SeedGun(btDiscreteDynamicsWorld* curworld);
	~SeedGun();

	void virtual UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner);

	void ReloadWeapon();
};
