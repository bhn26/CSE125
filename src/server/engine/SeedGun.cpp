#include "SeedGun.h"

SeedGun::SeedGun(btDiscreteDynamicsWorld* curworld): Weapon(gunfireRate, gunDamage, curworld)
{
	wt = WeaponType::SEEDGUN;
	nextFireTick = 0;
	gunfireRate = 5;
	gunDamage = 15;
	currentAmmo = 12;
	maxAmmo = 12;
	gunSpeed = new btVector3(0, 5, 25);
}

SeedGun::~SeedGun(){}

int SeedGun::UseWeapon(btVector3* position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{
	//printf("SeedGun has been fired!");
	if (this->fireFlag)
	{
		// Spawns bullet with this gun's damage, speed, and necessary ids into world
		Bullet* fireProjectile = EntitySpawner::instance()->spawnBullet(playerid, teamid, this->gunDamage, position, this->gunSpeed, rotation, curWorld);
		this->fireFlag = 0;
		this->nextFireTick = FireRateReset::instance()->currentWorldTick + gunfireRate;

		// add used weapon to "used" list in FireRateReset static object
		FireRateReset::instance()->AddWeapon(this);
		currentAmmo--;
	}
	return currentAmmo;
}
