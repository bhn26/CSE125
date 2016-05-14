#pragma once

#include "Weapon.h"

class SeedGun : public Weapon
{
protected:
	int gunfireRate = 100;
	int gunDamage = 10;

	// resets the reloaded flag when tick reaches nextFireTick

public:
	SeedGun(FireRateReset* frreset);
	~SeedGun();

	void UseWeapon(int playerId);

	void ReloadWeapon();
};