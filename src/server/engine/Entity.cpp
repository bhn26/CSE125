

#include "Entity.h"

Entity::Entity(int objectid, btDiscreteDynamicsWorld* physicsworld)
{
	this->id = objectid;
	this->curWorld = physicsworld;
}

Entity::~Entity(){};

btVector3 Entity::GetEntityPosition() { 
	return entityRigidBody->getCenterOfMassPosition(); 
}

btQuaternion Entity::GetEntityRotation() { 
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btQuaternion currentOrientation = currentTrans.getRotation();
	return currentOrientation;
}

int Entity::GetObjectId()
{
	return id;
}
