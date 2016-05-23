

#include "SeedGun.h"

SeedGun::SeedGun(btDiscreteDynamicsWorld* curworld): Weapon(gunfireRate, gunDamage, curworld)
{
	nextFireTick = 0;
}

SeedGun::~SeedGun(){}

void SeedGun::UseWeapon(const btVector3* position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{
	if (this->fireFlag)
	{
		// Spawns bullet with this gun's damage and ids into world
		btVector3* newVelocity = &((*rotation) * (*this->gunSpeed));
		Bullet* fireProjectile = EntitySpawner::instance()->spawnBullet(playerid, teamid, damage, position, newVelocity, curWorld);
		this->fireFlag = 0;
		this->nextFireTick = currentWorldTick + gunfireRate;

		// add used weapon to "used" list in FireRateReset static object
		std::shared_ptr<Weapon> weapon = std::shared_ptr<Weapon>(this);
		FireRateReset::instance()->AddWeapon(weapon);
	}
}

//TODO reload weapon clip?
void SeedGun::ReloadWeapon()
{
}