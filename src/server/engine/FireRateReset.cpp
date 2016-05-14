
#include "FireRateReset.h"

void FireRateReset::AddUsedWeapon(Weapon* usedWeapon)
{
	std::shared_ptr<Weapon> weapon = std::shared_ptr<Weapon>(usedWeapon);
	usedWeapons.push_back(weapon);
}

void FireRateReset::ResetWeapons()
{
	std::vector<std::shared_ptr<Weapon> >::iterator it = usedWeapons.begin();
	Weapon* nextWeapon;

	while (it != usedWeapons.end())
	{
		nextWeapon = it->get();
		if (nextWeapon->nextFireTick == currentWorldTick)
		{
			nextWeapon->fireFlag = 1;
			usedWeapons.erase(it);
		}
		else
		{
			it++;
		}
	}
}

