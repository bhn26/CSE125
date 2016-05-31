#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class GrenadeLauncher : public Weapon
{
protected:
	int gunfireRate = 1;
	int gunDamage = 100;
	btVector3* gunSpeed = new btVector3(0, 15, 20);

	// resets the reloaded flag when tick reaches nextFireTick

public:
	GrenadeLauncher(btDiscreteDynamicsWorld* curworld);
	~GrenadeLauncher();

	void virtual UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner);

	void ReloadWeapon();
};
