#pragma once

#include "../../network/GameData.h"

class Weapon
{

private:
	int fireRate;
	int damage;

public:
	Weapon(int firerate, int wdamage);
	~Weapon();

	void virtual UseWeapon();

	void virtual ReloadWeapon();
};