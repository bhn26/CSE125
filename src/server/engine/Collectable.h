#pragma once

#include "Entity.h"
#include "Weapon.h"
#include "../../network/GameData.h"
class Player;

class Collectable : public Entity
{
private:
	Weapon* weapon;

public:
	void SetWeapon(Weapon* nw) { weapon = nw; }
	Weapon* GetWeapon() { return weapon; }
	Collectable(int objectid, PosInfo pos, btDiscreteDynamicsWorld* curworld, Weapon* wp);
	virtual ~Collectable();
	void virtual HandleCollect(Player* collidedPlayer);
};

