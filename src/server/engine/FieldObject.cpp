#include "FieldObject.h"
#include "Entity.h"


FieldObject::FieldObject(btVector3* origin,  btCollisionShape* fieldshape, Entity* fieldowner, btDiscreteDynamicsWorld* curworld)
{
	this->curWorld = curworld;
	fieldOwner = fieldowner;
	FieldGhostObject = new btPairCachingGhostObject();
	FieldGhostObject->setCollisionShape(fieldshape);
	FieldGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	curWorld->addCollisionObject(FieldGhostObject, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
	FieldGhostObject->setWorldTransform(btTransform(btQuaternion::getIdentity(), (*origin)));
}

FieldObject::~FieldObject(){}

int FieldObject::handleField()
{
	return 1;
}