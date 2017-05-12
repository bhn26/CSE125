#pragma once
#include "network/GameData.h"

class Player;

class PowerUp // powerup base class
{
protected:
    PowerUpType type;

public:
    PowerUpType getType() { return type; }
    void setType(PowerUpType type) { this->type = type; }
    virtual void applyPower(Player* p){}; // applies the powerup to the player
    virtual void removePower(Player* p){};
};