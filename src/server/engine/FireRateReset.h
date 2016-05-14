#pragma once

#include <vector>
#include <memory>
#include "Weapon.h"
#include "../../network/GameData.h"

class FireRateReset
{

protected:
	std::vector<std::shared_ptr<Weapon>> * usedWeapons;

public:

	FireRateReset(std::vector<std::shared_ptr<Weapon>> * usedweapons);

	~FireRateReset();

	void ResetWeapons();
};