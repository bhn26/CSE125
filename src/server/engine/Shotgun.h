#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class Shotgun : public Weapon
{
public:
    Shotgun(btDiscreteDynamicsWorld* curworld);
    ~Shotgun();

    virtual int UseWeapon(const btVector3& position,
                          const btMatrix3x3& rotation,
                          int playerid,
                          int teamid,
                          Entity* owner) override;
};