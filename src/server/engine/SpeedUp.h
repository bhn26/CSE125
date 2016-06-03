#pragma once
#include "Powerup.h"
#include "Player.h"

class SpeedUp : public Powerup
{
public:
	static const int speed_bonus = 8;
	static const int speed_bonusDuration = 400;

	//SpeedUp() { this->type = PowerupType::SPEEDUP; }
	SpeedUp() {this->type = PowerupType::HEALTHGAIN;}
	~SpeedUp() {}

	void applyPower(Player* p)  // applies the powerup to the player
	{
		p->SetBonusSpeed(speed_bonusDuration);
		p->SetPowerupDuration(speed_bonusDuration);
		p->EquipPower(this);
	};
	void removePower(Player* p)
	{
		p->SetBonusSpeed(0);
		p->SetPowerupDuration(0);
		p->ResetPower();
		delete this;
	};
};