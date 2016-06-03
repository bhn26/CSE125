#pragma once
#include "../../network/GameData.h"

class Player;

class Powerup   // powerup base class
{
	protected:
		PowerupType type;

	public:
		PowerupType getType() { return type; }
		void setType(PowerupType type) { this->type = type; }
		virtual void applyPower(Player* p) {}; // applies the powerup to the player
		virtual void removePower(Player* p) {};
};