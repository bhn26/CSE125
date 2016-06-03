#pragma once
#include "Powerup.h"
#include "Player.h"

class HealthGain : public Powerup
{
	public:
		HealthGain() { this->type = PowerupType::HEALTHGAIN; }
		~HealthGain() {}

		void applyPower(Player* p) {p->GainHP(20);}; // applies the powerup to the player
		void removePower(Player* p) {};
};