#include "EntitySpawner.h"
#include "Flag.h"
#include "Player.h"
#include "ObjectId.h"

Flag::Flag(int objid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
    : Entity(ClassId::Flag, objid, physicsWorld), m_posInfo(pos)
{
    btCollisionShape* playerShape = new btBoxShape(btVector3(1, 1, 1));

    // Create Flag physics object
    btDefaultMotionState* playerMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, 90, pos.z)));
    btScalar mass = 1;
    btVector3 playerInertia(0, 0, 0);
    playerShape->calculateLocalInertia(mass, playerInertia);
    btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(
        mass, playerMotionState, playerShape, playerInertia);
    btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
    pRigidBody->forceActivationState(DISABLE_DEACTIVATION);
    physicsWorld->addRigidBody(pRigidBody);

    // Set Flag's protected fields
    m_entityRigidBody = pRigidBody;

    // Set RigidBody to point to Flag
    pRigidBody->setUserPointer(this);
    pRigidBody->setUserIndex(static_cast<int>(ClassId::Flag));
}

Flag::~Flag()
{
    if (m_entityRigidBody)
    {
        printf("----------------------deleting flag body\n");
        m_curWorld->removeCollisionObject(m_entityRigidBody);
        delete m_entityRigidBody->getMotionState();
        delete m_entityRigidBody->getCollisionShape();
        delete m_entityRigidBody;
    }
}

void Flag::HandleCollectable(Player* collidedPlayer)
{
    // Give player flag AND remove flag from entity map
    // NOTE: Remove flag from physics world after Tick has been processed
    collidedPlayer->AcquireFlag(this);
    EntitySpawner::Instance()->RemoveEntity(ClassId::Flag, m_objectId);
}