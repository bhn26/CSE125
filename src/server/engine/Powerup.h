#pragma once
#include "network/GameData.h"

class Player;

class PowerUp // powerup base class
{
protected:
    PowerUpType type;

public:
    PowerUpType GetType() { return type; }
    void SetType(PowerUpType type) { this->type = type; }
    virtual void ApplyPower(Player* p){}; // applies the powerup to the player
    virtual void RemovePower(Player* p){};
};