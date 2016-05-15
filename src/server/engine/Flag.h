#pragma once
#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

#ifndef GAME_DATA
#define GAME_DATA
#include "../../network/GameData.h"
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

	btVector3 GetFlagPosition();

	PosInfo p;

	int GetObjectId();

};