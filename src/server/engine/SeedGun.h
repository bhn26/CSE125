#pragma once

#include "Weapon.h"

class SeedGun : public Weapon
{
protected:
	int gunfireRate = 100;
	int gunDamage = 10;
	btVector3* gunSpeed = new btVector3(0, 30, 0);

	// resets the reloaded flag when tick reaches nextFireTick

public:
	SeedGun(std::vector<std::shared_ptr<Weapon>>* frreset);
	~SeedGun();

	void UseWeapon(int playerId);

	void ReloadWeapon();
};