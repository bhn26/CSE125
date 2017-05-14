#pragma once

#include "Collectable.h"

#include "network/GameData.h"

class Flag : public Entity
{
public:
    PosInfo m_posInfo;

    Flag(int objid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

    ~Flag();

    void HandleCollectable(Player* collidedPlayer);
};