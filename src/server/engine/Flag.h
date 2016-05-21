#pragma once
#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

#include "../../network/GameData.h"


class Flag
{
	int id;
	btDiscreteDynamicsWorld* curWorld;
	btRigidBody* flagRigidBody;
	

public:

	Flag(int id, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Flag();

	btRigidBody* getRigidBody();

	btVector3 GetFlagPosition();

	PosInfo p;

	int GetObjectId();

};