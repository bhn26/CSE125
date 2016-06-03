#pragma once

#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "../../network/GameData.h"

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
	int team_id;
    FieldType type;
    btDiscreteDynamicsWorld* curWorld;

public:
    int oid;  // public var cuz we gotta go
	FieldObject(btVector3* origin, btCollisionShape* fieldshape, int team_id, btDiscreteDynamicsWorld* curworld);
	virtual ~FieldObject();

	btPairCachingGhostObject* GetFieldGhostObject() {return FieldGhostObject;}

	// handles field detection. Returns 1 when field is done, 0 if not
	virtual int handleField();

    FieldType GetFieldType() { return type; }
};


