

#include "SeedGun.h"

SeedGun::SeedGun(btDiscreteDynamicsWorld* curworld): Weapon(gunfireRate, gunDamage, curworld)
{
	nextFireTick = 0;
}

SeedGun::~SeedGun(){}

void SeedGun::UseWeapon(btVector3* position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{
	printf("SeedGun has been fired!");
	if (this->fireFlag)
	{
		printf("Position:  x: %f, y: %f, z: %f  \n", position->getX(), position->getY(), position->getZ());

		// Spawns bullet with this gun's damage, speed, and necessary ids into world
		Bullet* fireProjectile = EntitySpawner::instance()->spawnBullet(playerid, teamid, this->gunDamage, position, this->gunSpeed, rotation, curWorld);
		//this->fireFlag = 0;
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