#pragma once
#include "Powerup.h"
#include "Player.h"

class JumpUp : public Powerup
{
public:
	static const int jump_bonus = 30;
	static const int jump_bonus_duration = 1200;

	JumpUp() {this->type = PowerupType::JUMPUP;}
	~JumpUp() {}

	void applyPower(Player* p)  // applies the powerup to the player
	{ 
		p->SetBonusJump(jump_bonus);
		p->SetPowerupDuration(jump_bonus_duration);
		p->EquipPower(this);
	};
	void removePower(Player* p) 
	{
		p->SetBonusJump(0);
		p->SetPowerupDuration(0);
		p->ResetPower();
		delete this;
	};
};