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
	bool alive;   // am i alive?
	unsigned int death_time; // when did i die?

public:

	Player(int objectid, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Player();

	PosInfo GetPosition() { return position; };

	int GetJump() { return jumpSem; }
	void SetJump() { jumpSem = 0; }

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

	void SetCam(float trotx, float trotz);

	// How much damage did they take, what is the world tick when they took this damage?
	int takeDamage(int damage, unsigned int world_tick);

	void UsePeck();

	// Which tick did this player die on? Disperses the player's flags then schedules his respawn for later
	void HandleDeath(unsigned int death_tick);

	void Move(btVector3* changeVelocity);

	void SetEntityRotation(float x, float y, float z, float w);

	unsigned int GetDeathTime() { return death_time; }
	void SetDeathTime(unsigned int dt) { death_time = dt; }

	int GetHitPoints() { return hitPoints; }
	void SetHitPoints(int hp) { hitPoints = hp; }

	bool IsAlive() { return alive; }
	void SetAlive(bool a) { alive = a; }

};

