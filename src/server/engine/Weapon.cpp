
#include "Weapon.h"

Weapon::Weapon(int firerate, int wdamage, std::vector<std::shared_ptr<Weapon>>* frreset, btDiscreteDynamicsWorld* curWorld)
{
	this->curWorld;
	this->fireRate = firerate;
	this->damage = wdamage;
	this->fireFlag = 1;
	this->frReset = frreset;
}

Weapon::~Weapon(){}

// to be overwritten
void Weapon::UseWeapon(const btVector3* position, btMatrix3x3* rotation, int playerid, int teamid){}

// to be overwritten
void Weapon::ReloadWeapon(){}
