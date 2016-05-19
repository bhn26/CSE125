#include "EntitySpawner.h"
#include "ObjectId.h"
#include "Player.h"
#include "Flag.h"
#include "Weapon.h"


Player::Player(int objectid, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld) : Entity(objectid, physicsWorld) {

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

	// Set Entity and Player protected fields
	this->entityRigidBody = pRigidBody;
	this->id = objectid;
	this->teamId = teamid;
	this->jumpSem = 1;
	this->hitPoints = 100;
	this->flags = new std::vector<std::shared_ptr<Flag>>;
	this->position = pos;
	this->playerWeapon = nullptr;

	// Set RigidBody to point to Player
	pRigidBody->setUserPointer(this);
	pRigidBody->setUserIndex(PLAYER);
}

Player::~Player() {
	this->curWorld->removeCollisionObject(entityRigidBody);
	delete entityRigidBody->getMotionState();
	delete entityRigidBody->getCollisionShape();
	delete entityRigidBody;
	flags->clear();
	delete flags;
}

void Player::PrintPlayerVelocity()
{
	//Calculate new velocity
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	printf("current velocity %f, %f, %f\n", float(entityRigidBody->getLinearVelocity()[0]), float(playerRigidBody->getLinearVelocity()[1]), float(playerRigidBody->getLinearVelocity()[2]));
}

void Player::Move(btVector3* changeVelocity) {
	//Calculate new velocity
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	btVector3 newVelocity = currentOrientation * (*changeVelocity);

	// set new velocity
	entityRigidBody->setLinearVelocity(newVelocity);
	//printf("%d: world pos object = %f,%f,%f\n", id, float(currentTrans.getOrigin().getX()), float(currentTrans.getOrigin().getY()), float(currentTrans.getOrigin().getZ()));
	printf("current velocity %f, %f, %f\n", float(entityRigidBody->getLinearVelocity()[0]), float( playerRigidBody->getLinearVelocity()[1]), float(playerRigidBody->getLinearVelocity()[2]));
	//entityRigidBody->activate();
}

void Player::Rotate(float v_rotation, float h_rotation) {
	//position.v_rotation = v_rotation;
	//position.h_rotation = h_rotation;
}


btQuaternion Player::GetPlayerRotation()
{
	printf("ERROR!!  Should call GetEntityRotation()");
	return GetEntityRotation();
}

void Player::SetPlayerRotation(float x, float y, float z, float w)
{
	btQuaternion* playerRotation = new btQuaternion(x, y, z, w);
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	currentTrans.setRotation((*playerRotation));
	entityRigidBody->setCenterOfMassTransform(currentTrans);
}

void Player::JumpPlayer()
{
	if (jumpSem)
	{
		// Change jump semaphore, change upward y-axis velocity
		jumpSem = 0;
		btVector3 curVelocity = entityRigidBody->getLinearVelocity();
		// setting upward velocity to 5
		curVelocity[1] = 5;
		entityRigidBody->setLinearVelocity(curVelocity);
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

int Player::GetTeamId()
{
	return teamId;
}

 void Player::UseWeapon()
{
	// passes player position when using weapon
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	playerWeapon->UseWeapon(&(entityRigidBody->getCenterOfMassPosition()), &currentOrientation, this->id, this->teamId);
}

void Player::EquipWeapon(Weapon* newWeapon)
{
	this->playerWeapon = newWeapon;
}

bool Player::HasWeapon()
{
	if(this->playerWeapon)
		return true;
	else
		return false;
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
