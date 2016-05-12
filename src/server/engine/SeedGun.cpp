

#include "SeedGun.h"

SeedGun::SeedGun(): Weapon(gunfireRate, gunDamage)
{
	int reloaded = 1;
	unsigned int nextFireTick = 0;
}

void SeedGun::UseWeapon(int playerId)
{
	if (this->reloaded)
	{
		// fire seed gun
		//TODO Spawn bullet with this gun's damage and given playerId
		this->reloaded = 0;
		this->nextFireTick = currentWorldTick + gunfireRate;
	}
}

void SeedGun::ReloadWeapon()
{
	if (!(this->reloaded))
	{
		if (nextFireTick == currentWorldTick)
		{
			this->reloaded = 1;
		}
	}
}