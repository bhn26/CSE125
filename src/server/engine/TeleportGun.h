#pragma once
#include "Weapon.h"
#include "Player.h"

class TeleportGun : public Weapon
{
public:
    TeleportGun(btDiscreteDynamicsWorld* curworld);
    ~TeleportGun();

    virtual int UseWeapon(const btVector3& position,
                          const btMatrix3x3& rotation,
                          int playerid,
                          int teamid,
                          Entity* owner) override;
};