#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class SeedGun : public Weapon
{
protected:
	int gunfireRate = 100;
	int gunDamage = 10;
	btVector3* gunSpeed = new btVector3(0, 30, 0);

	// resets the reloaded flag when tick reaches nextFireTick

public:
	SeedGun(btDiscreteDynamicsWorld* curworld);
	~SeedGun();

	void virtual UseWeapon(const btVector3* position, btMatrix3x3* rotation, int playerid, int teamid);

	void ReloadWeapon();
};