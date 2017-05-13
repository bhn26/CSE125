#pragma once
#include "Basic/Singleton.h"

#include <vector>
#include <memory>
#include "Network/GameData.h"

class Weapon;

class FireRateReset : public Singleton<FireRateReset>
{
    friend class Singleton<FireRateReset>;

private:
    std::vector<Weapon*> m_usedWeapons;
    FireRateReset() = default;

public:
    unsigned int currentWorldTick = 0;

    ~FireRateReset();

    void ResetWeapons();

    void AddWeapon(Weapon* weapon);
};