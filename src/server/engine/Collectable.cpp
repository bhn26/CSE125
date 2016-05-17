
#include "EntitySpawner.h"

Collectable::Collectable(btDiscreteDynamicsWorld* curworld) : Entity(curworld)
{
	this->curWorld = curworld;
}

void Collectable::HandleCollectable(Player* collidedPlayer){}

Collectable::~Collectable() {};