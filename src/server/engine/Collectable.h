#pragma once

#include "Entity.h"
#include "Weapon.h"
#include "Powerup.h"
#include "../../network/GameData.h"
class Player;

class Collectable : public Entity
{
private:
	CollectType type;
	Weapon* weapon;
	Powerup* powerup;

public:
	void SetWeapon(Weapon* nw) { weapon = nw; }
	Weapon* GetWeapon() { return weapon; }
	Powerup* GetPowerup() {return powerup; }
	Collectable(int objectid, PosInfo pos, btDiscreteDynamicsWorld* curworld, Weapon* wp);
	Collectable(int objectid, PosInfo pos, btDiscreteDynamicsWorld* curworld, Powerup* powerup);
	virtual ~Collectable();
	void virtual HandleCollect(Player* collidedPlayer);
	CollectType getType() {return type;}
};

