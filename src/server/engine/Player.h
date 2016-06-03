#pragma once
#include <memory>
#include <vector>
#include "Entity.h"
#include "Weapon.h"

#include "../../network/GameData.h"

class Flag;

class Player : public Entity
{

private:

	int teamId;
	std::string name;

	PosInfo position;
	std::vector<Flag*> *flags;
	int jumpSem;
	int hitPoints;
	int stun_count;
	Weapon* playerWeapon;
	Weapon* peckWeapon;
	bool alive;   // am i alive?
	unsigned int death_time; // when did i die?

	// Base and Bonus movement
	int baseJump;
	int baseSpeed;
	int bonusJump;
	int bonusSpeed;

public:

	Player(int objectid, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Player();

	PosInfo GetPosition() { return position; };

	int GetJumpSem() { return jumpSem; }
	void SetJumpSem() { jumpSem = 0; }

	// Return Player Base + Bonus
	int GetPlayerSpeed() { return (baseSpeed + bonusSpeed); };
	int GetPlayerJump() { return (baseJump + bonusJump); };

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

	void DiscardWeapon();

	bool HasWeapon();
	WeaponType GetPlayerWeaponType() { return playerWeapon->GetWeaponType(); };
	Weapon* GetWeapon() { return playerWeapon; };

	int GetTeamId();

	int GetScore() { return flags->size(); };

	int GetHP() { return hitPoints; };

	void GainHP(int gain) { 
		if(hitPoints + gain > 100) 
			hitPoints = 100;
		else
			hitPoints += gain;
	}

	int GetStun() {return stun_count;}
	void SetStun(int stun) {stun_count = stun;}

	// How much damage did they take, what is the world tick when they took this damage?
	int takeDamage(int damage, unsigned int world_tick);

	void UsePeck();

	// Which tick did this player die on? Disperses the player's flags then schedules his respawn for later
	void HandleDeath(unsigned int death_tick);

	void Move(btVector3* changeVelocity) override;

	void SetCamAngle(float yos);

	unsigned int GetDeathTime() { return death_time; }
	void SetDeathTime(unsigned int dt) { death_time = dt; }

	int GetHitPoints() { return hitPoints; }
	void SetHitPoints(int hp) { hitPoints = hp; }

	bool IsAlive() { return alive; }
	void SetAlive(bool a) { alive = a; }

	void SetName(std::string n) { name = n; }
	std::string GetName() { return name; }

};

