

#include "Entity.h"

Entity::Entity(btDiscreteDynamicsWorld* physicsworld)
{
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
