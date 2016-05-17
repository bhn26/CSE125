#pragma once

#include "Entity.h";

class Collectable : public Entity
{
private:

public:
	Collectable(btDiscreteDynamicsWorld* curworld);
	virtual ~Collectable();
	void virtual HandleCollectable(Player* collidedPlayer);
};