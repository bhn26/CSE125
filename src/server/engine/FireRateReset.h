#pragma once

#include <vector>
#include <memory>
#include "../../network/GameData.h"

class Weapon;

class FireRateReset
{

protected:
	std::vector<std::shared_ptr<Weapon>> * usedWeapons;

public:

	FireRateReset(std::vector<std::shared_ptr<Weapon>> * usedweapons);

	~FireRateReset();

	void ResetWeapons();
};