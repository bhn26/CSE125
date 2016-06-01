#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class SeedGun : public Weapon
{
protected:
	int gunfireRate;
	int gunDamage;
	btVector3* gunSpeed;

	// resets the reloaded flag when tick reaches nextFireTick

public:
	SeedGun(btDiscreteDynamicsWorld* curworld);
	~SeedGun();

	int virtual UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner);
};
