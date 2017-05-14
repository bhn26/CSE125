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
    std::vector<btRigidBody*> m_mapObjects;
    btDiscreteDynamicsWorld* m_curWorld;
    std::vector<const char*> m_fileNames;

public:
    MapLoader(btDiscreteDynamicsWorld* dynamicsWorld);
    ~MapLoader();
    void LoadMap();
};