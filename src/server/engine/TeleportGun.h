#pragma once
#include "Weapon.h"
#include "Player.h"

class TeleportGun : public Weapon
{

public:
	TeleportGun(btDiscreteDynamicsWorld* curworld);
	~TeleportGun();

	int UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner);
};