#include "EntitySpawner.h"
#include "ObjectId.h"

#include "server\ServerGame.h"

#include "Player.h"
#include "Flag.h"
#include "Peck.h"


Player::Player(int objectid, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld) : Entity(ClassId::PLAYER, objectid, physicsWorld) 
{
	btCollisionShape* playerShape = new btCylinderShape(btVector3(1, 1, 1));
	// Create player physics object
	btDefaultMotionState*playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));
	btScalar mass = 1;
	btVector3 playerInertia(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(mass, playerMotionState, playerShape, playerInertia);
	btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
	// only allow rotation around Y-axis
	pRigidBody->forceActivationState(DISABLE_DEACTIVATION);
    pRigidBody->setDamping((btScalar)0.1, (btScalar)1);
	pRigidBody->setFriction((btScalar) 10);
	physicsWorld->addRigidBody(pRigidBody);

	// Set Player protected fields
	this->entityRigidBody = pRigidBody;
	this->teamId = teamid;
	this->jumpSem = 1;
	this->hitPoints = 100;
	this->flags = new std::vector<std::shared_ptr<Flag>>;
	this->position = pos;
	this->playerWeapon = nullptr;
	this->peckWeapon = new Peck(curWorld);

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
	//printf("current velocity %f, %f, %f\n", float(entityRigidBody->getLinearVelocity()[0]), float(playerRigidBody->getLinearVelocity()[1]), float(playerRigidBody->getLinearVelocity()[2]));
}

void Player::JumpPlayer()
{
	if (jumpSem)
	{
		// Change jump semaphore, change upward y-axis velocity
		jumpSem = 0;
		btVector3 curVelocity = entityRigidBody->getLinearVelocity();
		// setting upward velocity to 3
		curVelocity[1] = 3;
		entityRigidBody->setLinearVelocity(curVelocity);
	}
}

void Player::ResetJump()
{
	(this->jumpSem) = 1;
}

void Player::AcquireFlag(std::shared_ptr<Flag> flag) 
{
	// player collects flag, remove from entity list
	flags->push_back(flag);
	printf("FLAG ACQUIRED\n");

	// note - individual scores are updated with move packets
	ServerGame::instance()->IncScore(teamId, 1);
	ServerGame::instance()->sendScorePacket();

	// check if your team won
	int * scores = ServerGame::instance()->GetScores();
	printf("Team %d has %d out of %d eggs\n", teamId, scores[teamId], ServerGame::instance()->NumTotalEggs());
	if(scores[teamId] == ServerGame::instance()->NumTotalEggs()) {
		printf("sending game over packet\n");
		ServerGame::instance()->sendGameOverPacket(teamId);
	}
}

void Player::LoseFlags()
{
	ServerGame::instance()->DecScore(teamId, flags->size());

	// Change this, we need the flags to come out of the player back into the world
	flags->clear();
	ServerGame::instance()->sendScorePacket();
}

int Player::GetTeamId()
{
	return teamId;
}

 void Player::UseWeapon()
{
	// If player weapon doesn't exist, exit
	if(!playerWeapon)
	{
		return;
	}
	// passes player position when using weapon
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	playerWeapon->UseWeapon(&(entityRigidBody->getCenterOfMassPosition()), &currentOrientation, this->objectId, this->teamId, this);
	printf("player with objId: %d used weapon\n", objectId);
}

void Player::EquipWeapon(Weapon* newWeapon)
{
	this->playerWeapon = newWeapon;
}

bool Player::HasWeapon()
{
	return (this->playerWeapon != nullptr);
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

void Player::UsePeck()
{
	// passes player position when using weapon
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	peckWeapon->UseWeapon(&(entityRigidBody->getCenterOfMassPosition()), &currentOrientation, this->objectId, this->teamId, this);
	printf("player with objId: %d used peck! \n", objectId);
}
