#pragma once
#include "Flag.h"
#include <memory>
#include <vector>

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

#ifndef GAME_DATA
#define GAME_DATA
#include "../../network/GameData.h"
#endif

class Player 
{

private:

	int id;
	// team
	PosInfo position;
	btDiscreteDynamicsWorld* curWorld;
	btRigidBody* playerRigidBody;
	std::vector<std::shared_ptr<Flag>> *flags;
	int jumpSem;
	int HitPoints;

public:

	Player(int id, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Player();

	PosInfo GetPosition() { return position; };

	void Move(btVector3 *changeVelocity);

	// Rotates player according to mouse move
	void Rotate(float v_rotation, float h_rotation);

	//TODO *********************************
	void PrintPlayerVelocity();

	// Return current positioning of Player
	btVector3 GetPlayerPosition();

	// Return current rotation matrix of Player
	btMatrix3x3 GetPlayerRotation();

	// Makes the player jump
	void JumpPlayer();

	//Flag handling
	void AcquireFlag(std::shared_ptr<Flag> flag);

	void LoseFlags();
};

