#pragma once
#include "Weapon.h"
class BlastMine : public Weapon
{
private:
	static const int ttl = 8;
	static const int magnitude = 100;
	static const int explosion_size = 17;
	static const int stun = 12;
	static const bool nonFriendly = false;

public:
	BlastMine(btDiscreteDynamicsWorld* curworld);
	~BlastMine();

    virtual int UseWeapon(btVector3* position,
                          btMatrix3x3* rotation,
                          int playerid,
                          int teamid,
                          Entity* owner) override;
};