

#include "SeedGun.h"

SeedGun::SeedGun(FireRateReset* frreset): Weapon(gunfireRate, gunDamage, frreset)
{
	int reloaded = 1;
	nextFireTick = 0;
}

void SeedGun::UseWeapon(int playerId)
{
	if (this->fireFlag)
	{
		// fire seed gun
		//TODO Spawn bullet with this gun's damage and given playerId
		this->fireFlag = 0;
		this->nextFireTick = currentWorldTick + gunfireRate;
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