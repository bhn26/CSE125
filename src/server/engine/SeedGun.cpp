

#include "SeedGun.h"

SeedGun::SeedGun(std::vector<std::shared_ptr<Weapon>>* frreset, btDiscreteDynamicsWorld* curworld): Weapon(gunfireRate, gunDamage, frreset, curworld)
{
	int reloaded = 1;
	nextFireTick = 0;
}

SeedGun::~SeedGun(){}

void SeedGun::UseWeapon(const btVector3* position, btMatrix3x3* rotation, int playerid, int teamid)
{
	if (this->fireFlag)
	{
		// Spawns bullet with this gun's damage and ids into world
		btVector3* newVelocity = &((*rotation) * (*this->gunSpeed));
		Bullet* fireProjectile = new Bullet(oid2, playerid, teamid, damage, position, newVelocity, curWorld);
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
}