#include "Collectable.h"
#include "EntitySpawner.h"
#include "Player.h"


Collectable::Collectable(int objectid, PosInfo pos, btDiscreteDynamicsWorld* curworld) : Entity(objectid, curworld)
{

	this->curWorld = curworld;
	btCollisionShape* collectableShape = new btBoxShape(btVector3(1, 1, 1));

	// Create Flag physics object
	btDefaultMotionState*collectableMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));
	btScalar mass = 1;
	btVector3 collectableInertia(0, 0, 0);
	collectableShape->calculateLocalInertia(mass, collectableInertia);
	btRigidBody::btRigidBodyConstructionInfo collectableRigidBodyCI(mass, collectableMotionState, collectableShape, collectableInertia);
	btRigidBody* pRigidBody = new btRigidBody(collectableRigidBodyCI);
	curWorld->addRigidBody(pRigidBody);

	// Set Flag's protected fields
	this->id = id;
	this->entityRigidBody = pRigidBody;

	// Set RigidBody to point to Flag
	pRigidBody->setUserPointer(this);
	pRigidBody->setUserIndex(COLLECTABLE);
}

void Collectable::HandleCollect(Player* collidedPlayer)
{
	// Randomize what type of weapon or powerup that player would get...
	// limit 2 weapons
}

Collectable::~Collectable()
{
	this->curWorld->removeCollisionObject(entityRigidBody);
	delete entityRigidBody->getMotionState();
	delete entityRigidBody->getCollisionShape();
	delete entityRigidBody;
}