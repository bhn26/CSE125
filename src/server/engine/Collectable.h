#pragma once

#include "Entity.h";
#include "../../network/GameData.h"
class Player;

class Collectable : public Entity
{
private:

public:
	Collectable(int objectid, PosInfo pos, btDiscreteDynamicsWorld* curworld);
	virtual ~Collectable();
	void virtual HandleCollect(Player* collidedPlayer);
};

