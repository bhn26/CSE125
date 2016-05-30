#include "EntitySpawner.h"
#include "ObjectId.h"

#include "server\ServerGame.h"

#include "Player.h"
#include "Flag.h"
#include "Peck.h"
#include "SeedGun.h"
#include <time.h>
#include "../ServerGame.h"


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
	this->flags = new std::vector<Flag*>;
	this->position = pos;
	//this->playerWeapon = nullptr;
	this->playerWeapon = new Peck(curWorld); 
	//this->playerWeapon = new SeedGun(curWorld);


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
		curVelocity[1] = 25;
		entityRigidBody->setLinearVelocity(curVelocity);
	}
}

void Player::ResetJump()
{
	(this->jumpSem) = 1;
}

void Player::AcquireFlag(Flag* flag)
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
	printf("Player %u : attempting to use weapon\n", objectId);
	// If player weapon doesn't exist, exit
	if(!playerWeapon)
	{
		return;
	}

	// passes player position when using weapon
	btVector3 temp = this->GetEntityPosition();
	printf("TEMP Position:  x: %f, y: %f, z: %f  \n", temp.getX(), temp.getY(), temp.getZ());

	btVector3* position = new btVector3(temp.getX(), temp.getY(), temp.getZ());
	btTransform currentTrans;
	entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();
	//btQuaternion* playerRotation = new btQuaternion(currentOrientation.getX(), currentOrientation.getY(), currentOrientation.getX(), currentOrientation.getW());

	playerWeapon->UseWeapon(position, &currentOrientation, this->objectId, this->teamId, this);
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

	printf("Player %u has taken damage!  Hitpoints:%d, damage: %d\n", objectId, this->hitPoints, damage);
	if (this->hitPoints <= 0)
	{
		this->HandleDeath();
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
//	peckWeapon->UseWeapon(&(entityRigidBody->getCenterOfMassPosition()), &currentOrientation, this->objectId, this->teamId, this);
	printf("player with objId: %d used peck! \n", objectId);
}

void Player::HandleDeath()
{
	printf("Player %u has died!", objectId);
	// reset hitpoints
	this->hitPoints = 100;

	btVector3 deathPos = entityRigidBody->getCenterOfMassPosition();
	deathPos.setY((deathPos.getY() + 4));
	srand(time(NULL));
	btTransform currentTrans;
	btVector3 ranVelocity;

	//std::vector<Flag*>::iterator it = flags->begin();

	//while (it != flags->end())
	for (auto it = flags->begin(); it != flags->end(); it++)
	{
		Flag* curFlag = (*it);
		curFlag->GetRigidBody()->getMotionState()->getWorldTransform(currentTrans);
		currentTrans.setOrigin(deathPos);
		curFlag->GetRigidBody()->getMotionState()->setWorldTransform(currentTrans);
		curFlag->GetRigidBody()->setCenterOfMassTransform(currentTrans);
		deathPos.setY((deathPos.getY() + 4));
		ranVelocity = btVector3((rand() % 14), (rand() % 14), (rand() % 14));
		printf("random velocity:  x: %f, y: %f, z: %f  \n", ranVelocity.getX(), ranVelocity.getY(), ranVelocity.getZ());

		// add Flag to world
		curWorld->addRigidBody(curFlag->GetRigidBody());
		curFlag->GetRigidBody()->setLinearVelocity(ranVelocity);
		// add Flag to client
		btQuaternion quat = curFlag->GetEntityRotation();
		PosInfo out;
		out.cid = ClassId::FLAG;
		out.oid = curFlag->GetObjectId();
		out.x = deathPos.getX();
		out.y = deathPos.getY();
		out.z = deathPos.getZ();
		out.rotw = quat.getW();
		out.rotx = quat.getX();
		out.roty = quat.getY();
		out.rotz = quat.getZ();
		ServerGame::instance()->sendSpawnPacket(out);
		// add Flag to Entity Map
		EntitySpawner::instance()->AddEntity(curFlag->GetClassId(), curFlag->GetObjectId(), curFlag);
		
		// erase flag from player
		//flags->erase(it);
	}
	flags->clear(); //Actually calls delete on flags... didn't seem to correctly work for bullet deletion anways... maybe cause of void pointer


	// Teleport Player to a random spot in the world
	btVector3 ranPos = btVector3((rand() % WORLD_WIDTH), 25, (rand() % WORLD_WIDTH));
	this->GetRigidBody()->getMotionState()->getWorldTransform(currentTrans);
	currentTrans.setOrigin(ranPos);
	this->GetRigidBody()->getMotionState()->setWorldTransform(currentTrans);
	this->GetRigidBody()->setCenterOfMassTransform(currentTrans);
}
