#pragma once


#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif



class WorldObstacle
{
private:
	int id;
	btRigidBody* obstacleRigidBody;
	btDiscreteDynamicsWorld* curWorld;

public:

	WorldObstacle(int objectid, btRigidBody* rigidbody, btDiscreteDynamicsWorld* curworld);
	~WorldObstacle();

	// return object id
	int GetObjectId();

	// returns pointer to obstacle rigid body
	btRigidBody* GetObjectRigidBody();
};