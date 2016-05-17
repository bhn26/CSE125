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

public:

	Entity(btDiscreteDynamicsWorld* physicsworld);

	virtual ~Entity();

	btVector3 GetEntityPosition();

	// Return current rotation quaternion of Player
	btQuaternion GetEntityRotation();
};