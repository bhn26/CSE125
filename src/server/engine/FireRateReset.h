#pragma once

#include <vector>
#include <memory>
#include "../../network/GameData.h"

class Weapon;

class FireRateReset
{

private:
	std::vector<std::shared_ptr<Weapon>> usedWeapons;
	static FireRateReset *frr;
	FireRateReset();

public:

	~FireRateReset();

	static FireRateReset *instance();

	void ResetWeapons();

	void AddWeapon(std::shared_ptr<Weapon> weapon);
};