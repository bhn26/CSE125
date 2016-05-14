
#include "Weapon.h"

Weapon::Weapon(int firerate, int wdamage, std::vector<std::shared_ptr<Weapon>>* frreset)
{
	this->fireRate = firerate;
	this->damage = wdamage;
	this->fireFlag = 1;
	this->frReset = frreset;
}

Weapon::~Weapon(){}

// to be overwritten
void Weapon::UseWeapon(){}

// to be overwritten
void Weapon::ReloadWeapon(){}
