#include "ServerPlayer.h"


ServerPlayer::ServerPlayer(int id, btDiscreteDynamicsWorld* physicsWorld)
{
	btCollisionShape* playerShape = new btCylinderShape(btVector3(1, 1, 1));

	btDefaultMotionState*playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2, 0)));
	btScalar mass = 1;
	btVector3 playerInertia(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(mass, playerMotionState, playerShape, playerInertia);
	btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
	physicsWorld->addRigidBody(pRigidBody);

	// Set Player's protected fields
	curWorld = physicsWorld;
	playerId = id;
	playerRigidBody = pRigidBody;
	jumpSem = 1;
}

ServerPlayer::~ServerPlayer()
{
	curWorld->removeCollisionObject(playerRigidBody);
	delete playerRigidBody->getMotionState();
	delete playerRigidBody->getCollisionShape();
}

void ServerPlayer::movePlayer(btVector3* changeVelocity)
{
	//Calculate new velocity
	btTransform currentTrans;
	playerRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	btVector3 newVelocity = currentOrientation * (*changeVelocity);

	// set new velocity
	playerRigidBody->setLinearVelocity(newVelocity);
}

void ServerPlayer::jumpPlayer()
{
	if (jumpSem)
	{
		// Change jump semaphore, change upward y-axis velocity
		jumpSem = 0;
		btVector3 curVelocity = playerRigidBody->getLinearVelocity();
		// setting upward velocity to 5
		curVelocity[1] = 5;
		playerRigidBody->setLinearVelocity(curVelocity);
	}
}

btVector3 ServerPlayer::getPlayerPosition()
{
	return playerRigidBody->getCenterOfMassPosition();
}

btMatrix3x3 ServerPlayer::getPlayerRotation()
{
	btTransform currentTrans;
	playerRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	return currentOrientation;
}
