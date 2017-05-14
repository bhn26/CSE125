#pragma once
#include "Player.h"
#include "PowerUp.h"

class HealthGain : public PowerUp
{
public:
    HealthGain() { this->type = PowerUpType::HealthGain; }
    ~HealthGain() {}
    void ApplyPower(Player* p) override { p->GainHP(20); }; // applies the powerup to the player
    void RemovePower(Player* p) override {};
};