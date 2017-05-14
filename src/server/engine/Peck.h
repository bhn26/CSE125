#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class Peck : public Weapon
{
protected:
    int m_meleeFireRate = 50;
    int m_meleeDamage = 32;

public:
    Peck(btDiscreteDynamicsWorld* curWorld);
    ~Peck();

    void ReloadWeapon();

    // always returns 1 because infinite ammo
    int UseWeapon(const btVector3& position,
                  const btMatrix3x3& rotation,
                  int playerid,
                  int teamid,
                  Entity* owner) override;
};