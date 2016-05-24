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

	void Move(btVector3* changeVelocity);

	// Return current rotation quaternion of Player
	btQuaternion GetEntityRotation();

	void SetEntityRotation(float x, float y, float z, float w);

	// Returns object id
	int GetObjectId();

	int GetClassId();

};