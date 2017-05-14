#pragma once
#include "Weapon.h"
#include "EntitySpawner.h"

class BounceGun : public Weapon
{
private:
    // this is more like bounceframes, it seems to be colliding again and again
    static const int NUM_BOUNCES = 5;

public:
    BounceGun(btDiscreteDynamicsWorld* curworld);
    ~BounceGun();

    virtual int UseWeapon(const btVector3& position,
                          const btMatrix3x3& rotation,
                          int playerid,
                          int teamid,
                          Entity* owner) override;
};
