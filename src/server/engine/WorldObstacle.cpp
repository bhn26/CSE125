
#include "WorldObstacle.h"

WorldObstacle::WorldObstacle(int objectid, btRigidBody* rigidbody, btDiscreteDynamicsWorld* curworld)
{
	this->id = objectid;
	this->obstacleRigidBody = rigidbody;
	this->curWorld = curworld;
}

WorldObstacle::~WorldObstacle()
{
	// should we delete Rigid body here?  or elswhere...
	this->curWorld->removeCollisionObject(obstacleRigidBody);
	delete obstacleRigidBody->getMotionState();
	delete obstacleRigidBody->getCollisionShape();
	delete obstacleRigidBody;
}

int WorldObstacle::GetObjectId()
{
	return this->id;
}

btRigidBody* WorldObstacle::GetObjectRigidBody()
{
	return this->obstacleRigidBody;
}
