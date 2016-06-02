#include "FieldObject.h"
#include "ObjectId.h"
#include "Entity.h"


FieldObject::FieldObject(btVector3* origin,  btCollisionShape* fieldshape, int team_id, btDiscreteDynamicsWorld* curworld)
{
	this->curWorld = curworld;
	this->team_id = team_id;
	FieldGhostObject = new btPairCachingGhostObject();
	FieldGhostObject->setCollisionShape(fieldshape);
	FieldGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	FieldGhostObject->setUserIndex(FIELD);
	curWorld->addCollisionObject(FieldGhostObject, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
	FieldGhostObject->setWorldTransform(btTransform(btQuaternion::getIdentity(), (*origin)));
}

FieldObject::~FieldObject(){}

int FieldObject::handleField()
{
	return 1;
}