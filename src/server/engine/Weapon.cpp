
#include "Weapon.h"

Weapon::Weapon(int firerate, int wdamage, btDiscreteDynamicsWorld* curWorld)
{
	this->curWorld = curWorld;
	this->fireRate = firerate;
	this->damage = wdamage;
	this->fireFlag = 1;
}

Weapon::~Weapon(){}

// to be overwritten
int Weapon::UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner) { return 0; }

// to be overwritten
void Weapon::ReloadWeapon() {currentAmmo = maxAmmo;}
