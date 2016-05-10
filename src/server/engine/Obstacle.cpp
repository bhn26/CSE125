
#include "Obstacle.h"

Obstacle::Obstacle(int objectid, btRigidBody* rigidbody, btDiscreteDynamicsWorld* curworld)
{
	this->id = objectid;
	this->obstacleRigidBody = rigidbody;
	this->curWorld = curworld;
}

Obstacle::~Obstacle()
{
	// should we delete Rigid body here?  or elswhere...
	this->curWorld->removeCollisionObject(obstacleRigidBody);
	delete obstacleRigidBody->getMotionState();
	delete obstacleRigidBody->getCollisionShape();
	delete obstacleRigidBody;
}

int Obstacle::GetObjectId()
{
	return this->id;
}

btRigidBody* Obstacle::GetObjectRigidBody()
{
	return this->obstacleRigidBody;
}
