
#include "FireRateReset.h"
#include "Weapon.h"
#include "../../network/GameData.h"


FireRateReset* FireRateReset::frr = nullptr;

FireRateReset* FireRateReset::instance()
{
	if (!frr)
	{
		frr = new FireRateReset();
	}
	return frr;
}

FireRateReset::FireRateReset()
{
	usedWeapons = new std::vector<Weapon*>;
	currentWorldTick = 0;
}

FireRateReset::~FireRateReset(){}

void FireRateReset::ResetWeapons()
{
	auto it = usedWeapons->begin();
	// Check through list of used weapons
	while (it != usedWeapons->end())
	{
		printf("checking weapon.  Current World Tick:  %u, nextFireTick %u \n", currentWorldTick, (*it)->nextFireTick);

		// reset fire flag if it can be fired again
		if ((*it)->nextFireTick == currentWorldTick)
		{
			(*it)->fireFlag = 1;
			it = usedWeapons->erase(it);
		}
		else
		{
			it++;
		}
	}
}

void FireRateReset::AddWeapon(Weapon* weapon)
{
	this->usedWeapons->push_back(weapon);
}
