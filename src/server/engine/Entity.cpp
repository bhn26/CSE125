#include "Entity.h"

Entity::Entity(ClassId classid, int objectid, btDiscreteDynamicsWorld* physicsworld)
    : m_classId(classid), m_objectId(objectid), m_curWorld(physicsworld)
{
}

Entity::~Entity(){};

btVector3 Entity::GetEntityPosition()
{
    return m_entityRigidBody->getCenterOfMassPosition();
}

void Entity::SetEntityPosition(btVector3* newPosition)
{
    btTransform currentTrans;
    m_entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
    currentTrans.setOrigin(*newPosition);
    m_entityRigidBody->getMotionState()->setWorldTransform(currentTrans);
    m_entityRigidBody->setCenterOfMassTransform(currentTrans);
}

void Entity::Move(btVector3* changeVelocity)
{
    // Calculate new velocity
    btTransform currentTrans;
    m_entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
    btMatrix3x3 currentOrientation = currentTrans.getBasis();
    btVector3 newVelocity = currentOrientation * (*changeVelocity);

    // set new velocity
    m_entityRigidBody->setLinearVelocity(newVelocity);
}

btVector3 Entity::GetEntityVelocity()
{
    return m_entityRigidBody->getLinearVelocity();
}

btQuaternion Entity::GetEntityRotation()
{
    btTransform currentTrans;
    m_entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
    btQuaternion currentOrientation = currentTrans.getRotation();
    return currentOrientation;
}

void Entity::SetEntityRotation(float x, float y, float z, float w)
{
    btQuaternion* playerRotation = new btQuaternion(x, y, z, w);
    btTransform currentTrans;
    m_entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
    currentTrans.setRotation((*playerRotation));
    m_entityRigidBody->getMotionState()->setWorldTransform(currentTrans);
    m_entityRigidBody->setCenterOfMassTransform(currentTrans);
}

int Entity::GetObjectId()
{
    return m_objectId;
}

ClassId Entity::GetClassId()
{
    return m_classId;
}

// Marks this entity to be deleted and ignored
void Entity::SetToMarked(unsigned int markTick)
{
    m_toDelete = 1;
    m_markTick = markTick;
}

void Entity::ResetMark()
{
    m_toDelete = 0;
}

// Checks if this entity is set to be deleted, sets collision detection to ignore
int Entity::GetMarked()
{
    return m_toDelete;
}

btRigidBody* Entity::GetRigidBody()
{
    return this->m_entityRigidBody;
}