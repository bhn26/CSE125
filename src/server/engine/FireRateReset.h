#pragma once

#include "Weapon.h"
#include <vector>
#include <memory>

class FireRateReset
{
protected:
	std::vector<std::shared_ptr<Weapon>> usedWeapons;

public:

	void AddUsedWeapon(Weapon*);

	void ResetWeapons();
};