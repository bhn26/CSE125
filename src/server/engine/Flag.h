#pragma once

#include "Entity.h"

#include "../../network/GameData.h"


class Flag : public Entity
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