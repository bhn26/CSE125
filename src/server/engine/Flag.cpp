#include "EntitySpawner.h"
#include "Flag.h"
#include "Player.h"
#include "ObjectId.h"

Flag::Flag(int objid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld): Entity(ClassId::FLAG, objid, physicsWorld)
{
	p = pos;

	btCollisionShape* playerShape = new btBoxShape(btVector3(1, 1, 1));

	// Create Flag physics object
	btDefaultMotionState*playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));
	btScalar mass = 1;
	btVector3 playerInertia(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(mass, playerMotionState, playerShape, playerInertia);
	btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
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
		this->curWorld->removeCollisionObject(entityRigidBody);
		delete entityRigidBody->getMotionState();
		delete entityRigidBody->getCollisionShape();
		delete entityRigidBody;
	}
}

btRigidBody* Flag::getRigidBody()
{
	return this->entityRigidBody;
}

void Flag::HandleCollectable(Player* collidedPlayer)
{
	// add 'this' to player
	// delete flag from EntitySpawnermap
	collidedPlayer->AcquireFlag(std::shared_ptr<Flag>(this));
	this->curWorld->removeCollisionObject(entityRigidBody);
	delete entityRigidBody->getMotionState();
	delete entityRigidBody->getCollisionShape();
	delete entityRigidBody;
	EntitySpawner::instance()->RemoveEntity(ClassId::FLAG, (objectId));
}