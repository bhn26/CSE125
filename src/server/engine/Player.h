#pragma once
#include <memory>
#include <vector>

#include "../../network/GameData.h"

class Entity;
class Flag;
class Weapon;

class Player : public Entity 
{

private:

	int id;
	int teamId;
	PosInfo position;
	btRigidBody* playerRigidBody;
	std::vector<std::shared_ptr<Flag>> *flags;
	int jumpSem;
	int hitPoints;
	Weapon* playerWeapon;

public:

	Player(int objectid, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Player();

	PosInfo GetPosition() { return position; };

	void Move(btVector3 *changeVelocity);

	// Rotates player according to mouse move
	void Rotate(float v_rotation, float h_rotation);

	//TODO *********************************
	void PrintPlayerVelocity();

	// Return current positioning of Player
	btVector3 GetPlayerPosition();

	// Return current rotation quaternion of Player
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

	void EquipWeapon(Weapon* newWeapon);

	bool HasWeapon();

	int GetTeamId();

	int takeDamage(int damage);
};

