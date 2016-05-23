

#include "Entity.h"

Entity::Entity(int classid, int objectid, btDiscreteDynamicsWorld* physicsworld)
{
	this->classId = classid;
	this->objectId = objectid;
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
	return objectId;
}

int Entity::GetClassId()
{
	return classId;
}
