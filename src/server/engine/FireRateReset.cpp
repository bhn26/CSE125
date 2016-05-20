
#include "FireRateReset.h"


FireRateReset::FireRateReset(std::vector<std::shared_ptr<Weapon>> * usedweapons)
{
	this->usedWeapons = usedweapons;
}

FireRateReset::~FireRateReset(){}

void FireRateReset::ResetWeapons()
{

	std::vector<std::shared_ptr<Weapon> >::iterator it = usedWeapons->begin();
	Weapon* nextWeapon;

	// Check through list of used weapons
	while (it != usedWeapons->end())
	{
		nextWeapon = it->get();
		// reset fire flag if it can be fired again
		if (nextWeapon->nextFireTick == currentWorldTick)
		{
			nextWeapon->fireFlag = 1;
			usedWeapons->erase(it);
		}
		else
		{
			// increment if weapon pointer wasn't removed
			it++;
		}
	}
}

