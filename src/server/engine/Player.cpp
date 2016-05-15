#include "Player.h"
#include "ObjectId.h"


Player::Player(int id, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld) {

	btCollisionShape* playerShape = new btCylinderShape(btVector3(1, 1, 1));
	// Create player physics object
	btDefaultMotionState*playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));
	btScalar mass = 1;
	btVector3 playerInertia(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(mass, playerMotionState, playerShape, playerInertia);
	btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
	pRigidBody->forceActivationState(DISABLE_DEACTIVATION);
	physicsWorld->addRigidBody(pRigidBody);

	// Set Player's protected fields
	this->id = id;
	this->teamId = teamid;
	this->curWorld = physicsWorld;
	this->playerRigidBody = pRigidBody;
	this->jumpSem = 1;
	this->hitPoints = 100;
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
	delete playerRigidBody;
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
	//playerRigidBody->activate();
}

void Player::Rotate(float v_rotation, float h_rotation) {
	//position.v_rotation = v_rotation;
	//position.h_rotation = h_rotation;
}

btVector3 Player::GetPlayerPosition()
{
	return playerRigidBody->getCenterOfMassPosition();
}

btQuaternion Player::GetPlayerRotation()
{
	btTransform currentTrans;
	playerRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btQuaternion currentOrientation = currentTrans.getRotation();
	return currentOrientation;
}

void Player::SetPlayerRotation(float x, float y, float z, float w)
{
	btQuaternion* playerRotation = new btQuaternion(x, y, z, w);
	btTransform currentTrans;
	playerRigidBody->getMotionState()->getWorldTransform(currentTrans);
	currentTrans.setRotation((*playerRotation));
	playerRigidBody->setCenterOfMassTransform(currentTrans);
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

void Player::ResetJump()
{
	(this->jumpSem) = 1;
}

void Player::AcquireFlag(std::shared_ptr<Flag> flag)
{
	// need to remove the flag from the map
	flags->push_back(flag);
	printf("FLAG ACQUIRED\n");
}

void Player::LoseFlags()
{
	// Change this, we need the flags to come out of the player back into the world
	flags->clear();
}

int Player::GetObjectId()
{
	return id;
}

int Player::GetTeamId()
{
	return teamId;
}

 void Player::UseWeapon()
{
	playerWeapon->UseWeapon();
}

 // If player is dead, returns 1,  else returns 0
 int Player::takeDamage(int damage)
 {
	 this->hitPoints = this->hitPoints - damage;
	 if (this->hitPoints <= 0)
	 {
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
 }