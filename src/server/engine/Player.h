#pragma once
#include <memory>
#include <vector>
#include "Entity.h"
#include "Weapon.h"
#include "Powerup.h"

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
	int stun_count;
	Weapon* playerWeapon;
	Weapon* peckWeapon;
	bool alive;   // am i alive?
	unsigned int death_time; // when did i die?

	// Base and Bonus movement
	int baseJump;
	int baseSpeed;
	int speedPenalty;  // penalty on your speed
	int lowestSpeed;   // lower bound on speed
	int bonusJump;
	int bonusSpeed;
	int hitPoints;
	int powerupDuration;

	Powerup* power;

public:

	static const int maxHealth = 100;

	Player(int objectid, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	~Player();

	PosInfo GetPosition() { return position; };

	int GetJumpSem() { return jumpSem; }
	void SetJumpSem() { jumpSem = 0; }

	// Return Player Base + Bonus
	int GetPlayerSpeed() 
	{ 
		// check if you're too slow for the speed threshold
		if((baseSpeed +bonusSpeed - speedPenalty) < lowestSpeed)
			return lowestSpeed;
		else
			return (baseSpeed + bonusSpeed - speedPenalty); 
	};
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
	void LosePower();

	bool HasWeapon();
	WeaponType GetPlayerWeaponType() { return playerWeapon->GetWeaponType(); };
	Weapon* GetWeapon() { return playerWeapon; };

	bool HasPower();
	void EquipPower(Powerup* powerup);
	Powerup* GetPower() {return power;}
	void ResetPower() {power = nullptr;}

	int GetTeamId();

	int GetScore() { return flags->size(); };

	int GetHP() { return hitPoints; };

	void GainHP(int gain) { 
		if(hitPoints + gain > maxHealth) 
			hitPoints = maxHealth;
		else
			hitPoints += gain;
	}

	int GetPowerupDuration() {return powerupDuration;}
	void SetPowerupDuration(int dur) {powerupDuration = dur;}

	int GetBonusJump() {return bonusJump;}
	void SetBonusJump(int jump) {bonusJump = jump;}

	int GetBonusSpeed() {return bonusSpeed;}
	void SetBonusSpeed(int speed) {bonusSpeed = speed;}

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

