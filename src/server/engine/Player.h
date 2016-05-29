#pragma once
#include <memory>
#include <vector>
#include "Entity.h"

#include "../../network/GameData.h"

class Flag;
class Weapon;

class Player : public Entity 
{

private:

	int teamId;

	PosInfo position;
	std::vector<Flag*> *flags;
	int jumpSem;
	int hitPoints;
	Weapon* playerWeapon;
	Weapon* peckWeapon;

public:

	Player(int objectid, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Player();

	PosInfo GetPosition() { return position; };

	void Move(btVector3 *changeVelocity);

	// Rotates player according to mouse move
	void Rotate(float v_rotation, float h_rotation);

	//TODO *********************************
	void PrintPlayerVelocity();

	// Makes the player jump
	void JumpPlayer();

	// Resets player jump semaphore
	void ResetJump();

	//Flag handling
	void AcquireFlag(Flag* flag);

	void LoseFlags();

	// use weapon
	// Pass in current rotation and position
	void UseWeapon();

	void EquipWeapon(Weapon* newWeapon);

	bool HasWeapon();

	int GetTeamId();

	int GetScore() { return flags->size(); };

	int GetJumpSem() { return jumpSem; };

	int takeDamage(int damage);

	void UsePeck();

	void HandleDeath();
};

