#pragma once

#include "../../network/GameData.h"
#include "FireRateReset.h"

class Weapon
{

protected:
	int fireRate;
	int damage;
	FireRateReset* frReset;

public:

	unsigned int nextFireTick;
	int fireFlag;

	Weapon(int firerate, int wdamage, FireRateReset* frreset);
	~Weapon();

	void virtual UseWeapon();

	void virtual ReloadWeapon();
};