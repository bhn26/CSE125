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

public:

	Entity(btDiscreteDynamicsWorld* physicsworld);

	virtual ~Entity();

	btVector3 GetPlayerPosition();

	// Return current rotation quaternion of Player
	btQuaternion GetPlayerRotation();
};