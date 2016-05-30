
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
void Weapon::UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner) { printf("ERROR:  Parent Weapon usedWeapon was called!\n"); }

// to be overwritten
void Weapon::ReloadWeapon(){}
