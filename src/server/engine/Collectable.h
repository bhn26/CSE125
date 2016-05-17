#pragma once

#include "Entity.h";
class Player;

class Collectable : public Entity
{
private:
	int id;
public:
	Collectable(int id, btDiscreteDynamicsWorld* curworld);
	virtual ~Collectable();
	void virtual HandleCollectable(Player* collidedPlayer);
};