
#include "Weapon.h"

Weapon::Weapon(int firerate, int wdamage, FireRateReset* frreset)
{
	this->fireRate = firerate;
	this->damage = wdamage;
	this->fireFlag = 1;
	this->frReset = frreset;
}

Weapon::~Weapon()
{
}


void Weapon::UseWeapon()
{

}

void Weapon::ReloadWeapon()
{

}
