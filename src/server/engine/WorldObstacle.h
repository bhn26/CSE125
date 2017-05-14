#pragma once

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

class WorldObstacle
{
private:
    int m_id = -1;
    btRigidBody* m_obstacleRigidBody = nullptr;
    btDiscreteDynamicsWorld* m_curWorld = nullptr;

public:
    WorldObstacle(int objectid, btRigidBody* rigidbody, btDiscreteDynamicsWorld* curworld);
    ~WorldObstacle();

    // return object id
    int GetObjectId();

    // returns pointer to obstacle rigid body
    btRigidBody* GetObjectRigidBody();
};