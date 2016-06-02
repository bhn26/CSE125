#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class GrenadeLauncher : public Weapon
{
protected:
	// resets the reloaded flag when tick reaches nextFireTick

public:
	GrenadeLauncher(btDiscreteDynamicsWorld* curworld);
	~GrenadeLauncher();

	int UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner);

	static const int EXPLOSION_SIZE = 17;
	static const int EXPLOSION_DUR = 10;

	void ReloadWeapon();
};
