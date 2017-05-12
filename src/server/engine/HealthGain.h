#pragma once
#include "Player.h"
#include "PowerUp.h"

class HealthGain : public PowerUp
{
public:
    HealthGain() { this->type = PowerUpType::HealthGain; }
    ~HealthGain() {}
    void applyPower(Player* p) override { p->GainHP(20); }; // applies the powerup to the player
    void removePower(Player* p) override {};
};