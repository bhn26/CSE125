

#include "Entity.h"

Entity::Entity(btDiscreteDynamicsWorld* physicsworld)
{
	this->curWorld = physicsworld;
}

Entity::~Entity(){};

btVector3 Entity::GetPlayerPosition() { return btVector3{ 0, 0, 0 }; };

btQuaternion Entity::GetPlayerRotation() { return btQuaternion{ 0, 0, 0, 0 }; };
