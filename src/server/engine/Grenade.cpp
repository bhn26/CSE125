
#include "Grenade.h"
#include "DamageField.h"
#include "EntitySpawner.h"
#include "FieldHandler.h"

Grenade::Grenade(int objectid, int playerid, int teamid, int damage, btVector3* pos, btVector3* velocity, btMatrix3x3* rotation, btDiscreteDynamicsWorld* physicsWorld, Entity* owner) : Entity(ClassId::GRENADE, objectid, physicsWorld)
{
	btCollisionShape* grenadeShape = new btSphereShape(btScalar(0.5));

	// Create Grenade physics object
	btVector3 globalPos = ((*rotation) * (btVector3(0, 0, 3))) + (*pos);

	btDefaultMotionState*grenadeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), (globalPos)));
	btScalar mass = 1;
	btVector3 grenadeInertia(0, 0, 0);
	grenadeShape->calculateLocalInertia(mass, grenadeInertia);
	btRigidBody::btRigidBodyConstructionInfo grenadeRigidBodyCI(mass, grenadeMotionState, grenadeShape, grenadeInertia);
	btRigidBody* bRigidBody = new btRigidBody(grenadeRigidBodyCI);

	// set Grenade rotation
	btTransform currentTrans;
	bRigidBody->getMotionState()->getWorldTransform(currentTrans);
	currentTrans.setBasis((*rotation));
	bRigidBody->getMotionState()->setWorldTransform(currentTrans);
	bRigidBody->setCenterOfMassTransform(currentTrans);

	// set grenade velocity
	btVector3 newVelocity = (*rotation) * (*velocity);

	bRigidBody->setLinearVelocity(newVelocity);
	bRigidBody->forceActivationState(DISABLE_DEACTIVATION);
	// Add to physics world
	physicsWorld->addRigidBody(bRigidBody);

	// Set Bullet's protected fields
	this->playerId = playerid;
	this->teamId = teamid;
	this->damage = damage;
	this->entityRigidBody = bRigidBody;
	this->owner = owner;

	// Set RigidBody to point to Bullet
	bRigidBody->setUserPointer(this);
	bRigidBody->setUserIndex(GRENADE);
}

Grenade::~Grenade()
{
	this->curWorld->removeCollisionObject(entityRigidBody);
	delete entityRigidBody->getMotionState();
	delete entityRigidBody->getCollisionShape();
	delete entityRigidBody;
	EntitySpawner::instance()->RemoveEntity(ClassId::GRENADE, objectId);
	printf("Grenade has been removed from dynamic map\n");
}

btVector3 Grenade::GetGrenadePosition()
{
	return (this->entityRigidBody)->getCenterOfMassPosition();
}

int Grenade::GetPlayerId()
{
	return this->playerId;
}

int Grenade::GetTeamId()
{
	return this->teamId;
}

void Grenade::HandleExplosion()
{
	printf("grenade handle explosion is called\n");
	btVector3 temp = this->GetEntityPosition();
	btVector3* position = new btVector3(temp.getX(), temp.getY(), temp.getZ());

	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();

	btCollisionShape* expSphere = new btSphereShape(btScalar(17));
	DamageField* explosionField = new DamageField(1, this->damage, position, expSphere, owner, curWorld);

	//TODO: Add to damage field checker. Add this new field to the checker
	FieldHandler::instance()->AddField(explosionField);
}
