#include "EntitySpawner.h"
#include "Flag.h"
#include "Player.h"
#include "ObjectId.h"

Flag::Flag(int objid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld): Entity(ClassId::FLAG, objid, physicsWorld)
{
	p = pos;

	btCollisionShape* playerShape = new btBoxShape(btVector3(1, 1, 1));

	// Create Flag physics object
	btDefaultMotionState*playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, 90, pos.z)));
	btScalar mass = 1;
	btVector3 playerInertia(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(mass, playerMotionState, playerShape, playerInertia);
	btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
	pRigidBody->forceActivationState(DISABLE_DEACTIVATION);
	physicsWorld->addRigidBody(pRigidBody);

	// Set Flag's protected fields
	this->entityRigidBody = pRigidBody;

	// Set RigidBody to point to Flag
	pRigidBody->setUserPointer(this);
	pRigidBody->setUserIndex(FLAG);
}

Flag::~Flag()
{
    if (entityRigidBody)
	{
		printf("----------------------deleting flag body\n");
		this->curWorld->removeCollisionObject(entityRigidBody);
		delete entityRigidBody->getMotionState();
		delete entityRigidBody->getCollisionShape();
		delete entityRigidBody;
	}
	
}

void Flag::HandleCollectable(Player* collidedPlayer)
{
	// Give player flag AND remove flag from entity map
	// NOTE: Remove flag from physics world after Tick has been processed
	collidedPlayer->AcquireFlag(this);
	EntitySpawner::instance()->RemoveEntity(ClassId::FLAG, (objectId));
}