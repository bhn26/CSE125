

#include "Entity.h"

Entity::Entity(int classid, int objectid, btDiscreteDynamicsWorld* physicsworld)
{
	this->toDelete = 0;
	this->classId = classid;
	this->objectId = objectid;
	this->curWorld = physicsworld;
}

Entity::~Entity(){};

btVector3 Entity::GetEntityPosition() 
{ 
	return entityRigidBody->getCenterOfMassPosition(); 
}

void Entity::Move(btVector3* changeVelocity) 
{
	//Calculate new velocity
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	btVector3 newVelocity = currentOrientation * (*changeVelocity);

	// set new velocity
	entityRigidBody->setLinearVelocity(newVelocity);
}

btQuaternion Entity::GetEntityRotation()
{ 
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btQuaternion currentOrientation = currentTrans.getRotation();
	return currentOrientation;
}

void Entity::SetEntityRotation(float x, float y, float z, float w) 
{
	btQuaternion* playerRotation = new btQuaternion(x, y, z, w);
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	currentTrans.setRotation((*playerRotation));
	entityRigidBody->getMotionState()->setWorldTransform(currentTrans);
	entityRigidBody->setCenterOfMassTransform(currentTrans);
}


int Entity::GetObjectId()
{
	return objectId;
}

int Entity::GetClassId()
{
	return classId;
}

// Marks this entity to be deleted and ignored
void Entity::SetToDelete()
{
	this->toDelete = 1;
}

// Checks if this entity is set to be deleted, sets collision detection to ignore
int Entity::DeleteStatus()
{
	return (this->toDelete);
}


btRigidBody* Entity::GetRigidBody()
{
	return this->entityRigidBody;
}