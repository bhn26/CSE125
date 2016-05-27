#pragma once

#include "BulletCollision\CollisionDispatch\btGhostObject.h"

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

class Entity;

class FieldObject
{
protected:
	btPairCachingGhostObject* FieldGhostObject;
	Entity * fieldOwner;
	btDiscreteDynamicsWorld* curWorld;
	int fieldTtl;

public:
	FieldObject(btVector3* origin, btCollisionShape* fieldshape, Entity* fieldowner, btDiscreteDynamicsWorld* curworld, int ttl);
	virtual ~FieldObject();

	// handles field detection. Returns 1 when field is done, 0 if not
	virtual int handleField();

	int isAlive();
};


