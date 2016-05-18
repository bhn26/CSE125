#pragma once

#include "Collectable.h"

#include "../../network/GameData.h"


class Flag : public Entity
{

public:

	PosInfo p;

	Flag(int id, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Flag();

	btRigidBody* getRigidBody();

	void HandleCollectable(Player* collidedPlayer);

};