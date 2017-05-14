#pragma once
#include "Weapon.h"
class BlastMine : public Weapon
{
private:
    static const int m_ttl = 8;
    static const int m_magnitude = 100;
    static const int m_explosionSize = 17;
    static const int m_stun = 12;
    static const bool m_nonFriendly = false;

public:
    BlastMine(btDiscreteDynamicsWorld* curworld);
    ~BlastMine();

    virtual int UseWeapon(const btVector3& position,
                          const btMatrix3x3& rotation,
                          int playerid,
                          int teamid,
                          Entity* owner) override;
};