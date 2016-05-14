

#include "SeedGun.h"

SeedGun::SeedGun(std::vector<std::shared_ptr<Weapon>>* frreset): Weapon(gunfireRate, gunDamage, frreset)
{
	int reloaded = 1;
	nextFireTick = 0;
}

SeedGun::~SeedGun(){}

void SeedGun::UseWeapon(int playerId)
{
	if (this->fireFlag)
	{
		// fire seed gun
		//TODO Spawn bullet with this gun's damage and given playerId
		this->fireFlag = 0;
		this->nextFireTick = currentWorldTick + gunfireRate;

		// add used weapon to "used" list in World
		std::shared_ptr<Weapon> weapon = std::shared_ptr<Weapon>(this);
		frReset->push_back(weapon);
	}
}

//TODO change this to reload weapon clip?
void SeedGun::ReloadWeapon()
{
	if (!(this->fireFlag))
	{
		if (nextFireTick == currentWorldTick)
		{
			this->fireFlag = 1;
		}
	}
}