#pragma once

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

#include <vector>
#include <string>

class MapLoader
{
protected:
    std::vector<btRigidBody*> mapObjects;
    btDiscreteDynamicsWorld* curWorld;
    std::vector<const char*> fileNames;

public:
    MapLoader(btDiscreteDynamicsWorld* dynamicsWorld);
    ~MapLoader();
    void loadMap();
};