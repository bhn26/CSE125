#pragma once

#include "Weapon.h"
#include "EntitySpawner.h"

class Shotgun : public Weapon
{
public:
	Shotgun(btDiscreteDynamicsWorld* curworld);
	~Shotgun();

    virtual int UseWeapon(btVector3* position,
                          btMatrix3x3* rotation,
                          int playerid,
                          int teamid,
                          Entity* owner) override;
};