
#include "Bullet.h"
#include "EntitySpawner.h"

Bullet::Bullet(int objectid, int playerid, int teamid, int damage, btVector3* pos, btVector3* velocity, btMatrix3x3* rotation, btDiscreteDynamicsWorld* physicsWorld): Entity(ClassId::BULLET, objectid, physicsWorld)
{
	btCollisionShape* bulletShape = new btSphereShape(btScalar(0.5));

	//printf("Position:  x: %d, y: %d, z: %d  \n", pos->getX(), pos->getY(), pos->getZ());
	//printf("Velocity:  x: %d, y: %d, z: %d  \n", velocity->getX(), velocity->getY(), velocity->getZ());

	// Create bullet physics object
	btVector3 globalPos = ((*rotation) * (btVector3(0, 0, 3))) + (*pos);

	//printf("Current position:  x: %f, y: %f, z: %f  \n", pos->getX(), pos->getY(), pos->getZ());
	//printf("New position:  x: %f, y: %f, z: %f  \n", globalPos.getX(), globalPos.getY(), globalPos.getZ());

	btDefaultMotionState*bulletMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), (globalPos)));
	btScalar mass = 1;
	btVector3 bulletInertia(0, 0, 0);
	bulletShape->calculateLocalInertia(mass, bulletInertia);
	btRigidBody::btRigidBodyConstructionInfo bulletRigidBodyCI(mass, bulletMotionState, bulletShape, bulletInertia);
	btRigidBody* bRigidBody = new btRigidBody(bulletRigidBodyCI);

	// set bullet rotation
	btTransform currentTrans;
	bRigidBody->getMotionState()->getWorldTransform(currentTrans);
	currentTrans.setBasis((*rotation));
	bRigidBody->getMotionState()->setWorldTransform(currentTrans);
	bRigidBody->setCenterOfMassTransform(currentTrans);

	// set bullet velocity
	//printf("current velocity:  x: %f, y: %f, z: %f  \n", velocity->getX(), velocity->getY(), velocity->getZ());
	btVector3 newVelocity = (*rotation) * (*velocity);
	//printf("New velocity:  x: %f, y: %f, z: %f  \n", newVelocity.getX(), newVelocity.getY(), newVelocity.getZ());

	bRigidBody->setLinearVelocity(newVelocity);
	bRigidBody->forceActivationState(DISABLE_DEACTIVATION);
	// Add to physics world
	physicsWorld->addRigidBody(bRigidBody);

	// Set Bullet's protected fields
	this->playerId = playerid;
	this->teamId = teamid;
	this->damage = damage;
	this->entityRigidBody = bRigidBody;

	// Set RigidBody to point to Bullet
	bRigidBody->setUserPointer(this);
	bRigidBody->setUserIndex(BULLET);
}

Bullet::~Bullet()
{
	printf("deleting bullet!\n");
	this->curWorld->removeCollisionObject(entityRigidBody);
	delete entityRigidBody->getMotionState();
	delete entityRigidBody->getCollisionShape();
	delete entityRigidBody;
	EntitySpawner::instance()->RemoveEntity(ClassId::BULLET,objectId);
	printf("should have deleted bullet!\n");

}

btVector3 Bullet::GetBulletPosition()
{
	return (this->entityRigidBody)->getCenterOfMassPosition();
}

int Bullet::GetPlayerId()
{
	return this->playerId;
}

int Bullet::GetTeamId()
{
	return this->teamId;
}

int Bullet::GetDamage()
{
	return this->damage;
}
