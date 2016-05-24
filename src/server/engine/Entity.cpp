

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

void Entity::Move(btVector3* changeVelocity) {
	//Calculate new velocity
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	btQuaternion q = GetEntityRotation();
	//btVector3 newVelocity = btVector3(q.getW() * 3, 0, q.getY() * 3);
	btVector3 newVelocity = currentOrientation * (*changeVelocity);
	//printf("Q OF PLAYER MOVING IS :  %f, %f, %f, %f\n", q.getW(), q.getX(), q.getY(), q.getZ());
	// set new velocity
	entityRigidBody->setLinearVelocity(newVelocity);
	//printf("%d: world pos object = %f,%f,%f\n", id, float(currentTrans.getOrigin().getX()), float(currentTrans.getOrigin().getY()), float(currentTrans.getOrigin().getZ()));
}

btQuaternion Entity::GetEntityRotation() { 
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btQuaternion currentOrientation = currentTrans.getRotation();
	return currentOrientation;
}

void Entity::SetEntityRotation(float x, float y, float z, float w) {
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
