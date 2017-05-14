#pragma once

#include "server/engine/ObjectId.h"

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

class Entity
{
protected:
    btDiscreteDynamicsWorld* m_curWorld = nullptr;
    btRigidBody* m_entityRigidBody = nullptr;
    int m_objectId = 0;
    ClassId m_classId;
    int m_toDelete = 0;
    unsigned int m_markTick = 0; // world tick that this thing was marked on
    btQuaternion m_cameraAngle;

public:
    Entity(ClassId classid, int objectid, btDiscreteDynamicsWorld* physicsworld);

    virtual ~Entity();

    btVector3 GetEntityPosition();

    void SetEntityPosition(btVector3* newPosition);

    virtual void Move(btVector3* changeVelocity);

    btVector3 GetEntityVelocity();

    // Return current rotation quaternion of Player
    btQuaternion GetEntityRotation();

    void SetEntityRotation(float x, float y, float z, float w);

    // Returns object id
    int GetObjectId();

    ClassId GetClassId();

    btDiscreteDynamicsWorld* GetPhysicsWorld() { return m_curWorld; }
    btRigidBody* GetRigidBody();

    // Marks this entity to be ignored
    void SetToMarked(unsigned int markTick);

    unsigned int GetMarkTick() { return m_markTick; }
    void ResetMark();

    // Checks if this entity is set to be deleted, sets collision detection to ignore
    int GetMarked();
};