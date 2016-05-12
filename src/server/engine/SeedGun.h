#pragma once

#include "Weapon.h"

class SeedGun : public Weapon
{
private:
	int gunfireRate = 100;
	int gunDamage = 10;

	// Variable to denote if the gun is ready to be fired
	int reloaded;
	// resets the reloaded flag when tick reaches nextFireTick
	unsigned int nextFireTick;

public:
	SeedGun();
	~SeedGun();

	void UseWeapon(int playerId);

	void ReloadWeapon();
};