#pragma once
#include "Weapon.h"
#include "EntitySpawner.h"

class Peck : public Weapon
{
protected:
	int meleefireRate = 10000;
	int meleeDamage = 25;
public:
	Peck(btDiscreteDynamicsWorld* curWorld);
	~Peck();

	void virtual UseWeapon(const btVector3* position, btMatrix3x3* rotation, int playerid, int teamid);
};