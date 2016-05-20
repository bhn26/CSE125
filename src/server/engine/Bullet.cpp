

#include "Bullet.h"
#include "../../network/GameData.h"

Bullet::Bullet(int objectid, int playerid, int teamid, int damage, PosInfo pos, btVector3* velocity, btDiscreteDynamicsWorld* physicsWorld)
{
	btCollisionShape* bulletShape = new btSphereShape(btScalar(.1));

	// Create bullet physics object
	btDefaultMotionState*bulletMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));
	btScalar mass = 1;
	btVector3 bulletInertia(0, 0, 0);
	bulletShape->calculateLocalInertia(mass, bulletInertia);
	btRigidBody::btRigidBodyConstructionInfo bulletRigidBodyCI(mass, bulletMotionState, bulletShape, bulletInertia);
	btRigidBody* bRigidBody = new btRigidBody(bulletRigidBodyCI);

	// set bullet velocity and add to physics world
	bRigidBody->setLinearVelocity((*velocity));
	bRigidBody->forceActivationState(DISABLE_DEACTIVATION);
	physicsWorld->addRigidBody(bRigidBody);

	// Set RigidBody to point to Bullet
	bRigidBody->setUserPointer(this);
	bRigidBody->setUserIndex(BULLET);

	this->id = objectid;
	this->playerId = playerid;
	this->teamId = teamid;
	this->damage = damage;
	this->curWorld = physicsWorld;
	this->bulletRigidBody = bRigidBody;
}

Bullet::~Bullet()
{
	this->curWorld->removeCollisionObject(bulletRigidBody);
	delete bulletRigidBody->getMotionState();
	delete bulletRigidBody->getCollisionShape();
	delete bulletRigidBody;
}

btVector3 Bullet::GetBulletPosition()
{
	return (this->bulletRigidBody)->getCenterOfMassPosition();
}

int Bullet::GetObjectId()
{
	return this->id;
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