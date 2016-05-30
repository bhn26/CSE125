#include "FieldObject.h"
#include "Entity.h"


FieldObject::FieldObject(btVector3* origin,  btCollisionShape* fieldshape, Entity* fieldowner, btDiscreteDynamicsWorld* curworld, int ttl)
{
	this->curWorld = curworld;
	fieldOwner = fieldowner;
	FieldGhostObject = new btPairCachingGhostObject();
	FieldGhostObject->setCollisionShape(fieldshape);
	FieldGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	curWorld->addCollisionObject(FieldGhostObject, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
	FieldGhostObject->setWorldTransform(btTransform(btQuaternion::getIdentity(), (*origin)));
	this->fieldTtl = ttl;
}

FieldObject::~FieldObject(){}

// 0 means remove field from queue
int FieldObject::handleField()
{
	return 0;
}

// returns 1 if field is still "alive"... not really necessary
int FieldObject::isAlive()
{
	return (fieldTtl != 0);
}