#include "Collectable.h"
#include "EntitySpawner.h"

Collectable::Collectable(int id, btDiscreteDynamicsWorld* curworld) : Entity(id, curworld)
{
	this->curWorld = curworld;
}

void Collectable::HandleCollectable(Player* collidedPlayer){}

Collectable::~Collectable() {};