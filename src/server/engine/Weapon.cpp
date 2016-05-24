
#include "Weapon.h"

Weapon::Weapon(int firerate, int wdamage, btDiscreteDynamicsWorld* curWorld)
{
	this->curWorld;
	this->fireRate = firerate;
	this->damage = wdamage;
	this->fireFlag = 1;
}

Weapon::~Weapon(){}

// to be overwritten
void Weapon::UseWeapon(const btVector3* position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner){}

// to be overwritten
void Weapon::ReloadWeapon(){}
