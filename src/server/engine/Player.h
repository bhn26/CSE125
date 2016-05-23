#pragma once
#include "Flag.h"
#include "Weapon.h"
#include <memory>
#include <vector>

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

#include "../../network/GameData.h"


class Player 
{

private:

	int id;
	int teamId;
	PosInfo position;
	btDiscreteDynamicsWorld* curWorld;
	btRigidBody* playerRigidBody;
	std::vector<std::shared_ptr<Flag>> *flags; // eggs collected 
	int jumpSem;
	int hitPoints;
	Weapon* playerWeapon;

public:

	Player(int id, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Player();

	int GetId() { return id; }

	PosInfo GetPosition() { return position; };

	void Move(btVector3 *changeVelocity);

	// Rotates player according to mouse move
	void Rotate(float v_rotation, float h_rotation);

	//TODO *********************************
	void PrintPlayerVelocity();

	// Return current positioning of Player
	btVector3 GetPlayerPosition();

	// Return current rotation matrix of Player
	btQuaternion GetPlayerRotation();

	// Sets player rotation using quarternion
	void SetPlayerRotation(float x, float y, float z, float w);

	// Makes the player jump
	void JumpPlayer();

	// Resets player jump semaphore
	void ResetJump();

	//Flag handling
	void AcquireFlag(std::shared_ptr<Flag> flag);

	void LoseFlags();

	// use weapon
	// Pass in current rotation and position
	void UseWeapon();

	int GetObjectId();

	int GetTeamId();

	int GetScore() { return flags->size(); };

	int takeDamage(int damage);
};

