#include "Player.h"
#include "ObjectId.h"


Player::Player(int id, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld) {

	btCollisionShape* playerShape = new btCylinderShape(btVector3(1, 1, 1));
	// Create player physics object
	btDefaultMotionState*playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));
	btScalar mass = 1;
	btVector3 playerInertia(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(mass, playerMotionState, playerShape, playerInertia);
	btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
	physicsWorld->addRigidBody(pRigidBody);

	// Set Player's protected fields
	this->id = id;
	this->curWorld = physicsWorld;
	this->playerRigidBody = pRigidBody;
	this->jumpSem = 1;
	this->HitPoints = 100;
	this->flags = new std::vector<std::shared_ptr<Flag>>;
	this->position = pos;

	// Set RigidBody to point to Player
	pRigidBody->setUserPointer(this);
	pRigidBody->setUserIndex(PLAYER);
}

Player::~Player() {
	this->curWorld->removeCollisionObject(playerRigidBody);
	delete playerRigidBody->getMotionState();
	delete playerRigidBody->getCollisionShape();
	flags->clear();
	delete flags;
}

void Player::PrintPlayerVelocity()
{
	//Calculate new velocity
	btTransform currentTrans;
	playerRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	printf("current velocity %f, %f, %f\n", float(playerRigidBody->getLinearVelocity()[0]), float(playerRigidBody->getLinearVelocity()[1]), float(playerRigidBody->getLinearVelocity()[2]));

}

void Player::Move(btVector3* changeVelocity) {
	//Calculate new velocity
	btTransform currentTrans;
	playerRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	btVector3 newVelocity = currentOrientation * (*changeVelocity);

	// set new velocity
	playerRigidBody->setLinearVelocity(newVelocity);
	//printf("%d: world pos object = %f,%f,%f\n", id, float(currentTrans.getOrigin().getX()), float(currentTrans.getOrigin().getY()), float(currentTrans.getOrigin().getZ()));
	printf("current velocity %f, %f, %f\n", float(playerRigidBody->getLinearVelocity()[0]), float( playerRigidBody->getLinearVelocity()[1]), float(playerRigidBody->getLinearVelocity()[2]));
	playerRigidBody->activate();

	/*position.direction = direction;
	 
	switch (direction) {
	case MOVE_FORWARD: 
		position.y ++;
		break;
	case MOVE_BACKWARD:
		position.y--;
		break;
	case MOVE_LEFT:
		position.x++;
		break;
	case MOVE_RIGHT:
		position.x--;
		break;
	}*/
}

void Player::Rotate(float v_rotation, float h_rotation) {
	position.v_rotation = v_rotation;
	position.h_rotation = h_rotation;
}

btVector3 Player::GetPlayerPosition()
{
	return playerRigidBody->getCenterOfMassPosition();
}

btMatrix3x3 Player::GetPlayerRotation()
{
	btTransform currentTrans;
	playerRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	return currentOrientation;
}

void Player::JumpPlayer()
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

void Player::AcquireFlag(std::shared_ptr<Flag> flag)
{
	// need to remove the flag from the map
	flags->push_back(flag);
	printf("Flag was acquired! \n");
}

void Player::LoseFlags()
{
	// Change this, we need the flags to come out of the player back into the world
	flags->clear();
}