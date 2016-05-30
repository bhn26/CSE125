#pragma once

#include <vector>
#include <memory>

class Weapon;

class FireRateReset
{

private:
	std::vector<Weapon*>* usedWeapons;
	static FireRateReset *frr;
	FireRateReset();

public:

	unsigned int currentWorldTick;

	~FireRateReset();

	static FireRateReset *instance();

	void ResetWeapons();

	void AddWeapon(Weapon* weapon);
};