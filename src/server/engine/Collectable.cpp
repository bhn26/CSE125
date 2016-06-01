#include "Collectable.h"
//#include "EntitySpawner.h"
#include "Player.h"
#include <time.h>

//List of weapons
#include "SeedGun.h"

Collectable::Collectable(int objectid, PosInfo pos, btDiscreteDynamicsWorld* curworld, Weapon* wp) : Entity(ClassId::COLLECTABLE, objectid, curworld)
{
	this->curWorld = curworld;
	this->weapon = wp;
	btCollisionShape* collectableShape = new btBoxShape(btVector3(1, 1, 1));

	// Create Collectable physics object
	btDefaultMotionState*collectableMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));
	btScalar mass = 1;
	btVector3 collectableInertia(0, 0, 0);
	collectableShape->calculateLocalInertia(mass, collectableInertia);
	btRigidBody::btRigidBodyConstructionInfo collectableRigidBodyCI(mass, collectableMotionState, collectableShape, collectableInertia);
	btRigidBody* pRigidBody = new btRigidBody(collectableRigidBodyCI);
	curWorld->addRigidBody(pRigidBody);

	// Set Collectable's protected fields
	this->entityRigidBody = pRigidBody;

	// Set RigidBody to point to Collectable
	pRigidBody->setUserPointer(this);
	pRigidBody->setUserIndex(COLLECTABLE);
}

//NOTE***  Version if Weapon is only thing collectable
void Collectable::HandleCollect(Player* collidedPlayer)
{
	//Remove collectable rigidBody
	this->curWorld->removeCollisionObject(entityRigidBody);
	delete entityRigidBody->getMotionState();
	delete entityRigidBody->getCollisionShape();
	delete entityRigidBody;

	//Remove collectable object from EntitySpawner Map
	EntitySpawner::instance()->RemoveEntity(ClassId::COLLECTABLE, objectId);

	// If player already has usable
	if (collidedPlayer->HasWeapon())
	{
		return;
	}

	// Otherwise, give random useable
	// Randomize what type of weapon or powerup that player would get
//	switch (ranPower)
//	{
//	}

	collidedPlayer->EquipWeapon(weapon);
}

Collectable::~Collectable()
{
	this->curWorld->removeCollisionObject(entityRigidBody);
	delete entityRigidBody->getMotionState();
	delete entityRigidBody->getCollisionShape();
	delete entityRigidBody;
}