#include "Bullet.h"
#include "EntitySpawner.h"

Bullet::Bullet(unsigned int objectid, int playerid, int teamid, int damage, BulletCollisionHandler* handler, btRigidBody* bullet_body, btDiscreteDynamicsWorld* physicsWorld) : Entity(ClassId::BULLET, objectid, physicsWorld)
{
	physicsWorld->addRigidBody(bullet_body);

	// Set Bullet's protected fields
	this->c_handler = handler;
	this->playerId = playerid;
	this->teamId = teamid;
	this->damage = damage;
	this->entityRigidBody = bullet_body;

	bullet_body->setUserPointer(this);
	bullet_body->setUserIndex(BULLET);
}

Bullet::~Bullet()
{
	this->curWorld->removeCollisionObject(entityRigidBody);
	delete entityRigidBody->getMotionState();
	delete entityRigidBody->getCollisionShape();
	delete entityRigidBody;
	delete c_handler;
	EntitySpawner::instance()->RemoveEntity(ClassId::BULLET, objectId);
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
