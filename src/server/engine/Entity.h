#pragma once

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif


class Entity
{

protected:

	btDiscreteDynamicsWorld* curWorld;
	btRigidBody* entityRigidBody;
	int objectId;
	int classId;

public:

	Entity(int classid, int objectid, btDiscreteDynamicsWorld* physicsworld);

	virtual ~Entity();

	btVector3 GetEntityPosition();

	// Return current rotation quaternion of Player
	btQuaternion GetEntityRotation();

	// Returns object id
	int GetObjectId();

	int GetClassId();

};