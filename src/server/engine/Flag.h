#pragma once
#include "../../network/GameData.h"

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

class Flag
{
	int id;
	btDiscreteDynamicsWorld* curWorld;
	btRigidBody* flagRigidBody;

public:

	Flag(int id, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Flag();

	btRigidBody* getRigidBody();

};