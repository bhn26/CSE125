#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class SeedGun : public Weapon
{
public:
	SeedGun(btDiscreteDynamicsWorld* curworld);
	~SeedGun();

	int virtual UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner);
};
