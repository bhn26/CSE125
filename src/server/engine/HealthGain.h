#pragma once
#include "Player.h"
#include "PowerUp.h"

class HealthGain : public PowerUp
{
public:
    HealthGain() { m_type = PowerUpType::HealthGain; }
    ~HealthGain() {}
    // applies the powerup to the player
    void ApplyPower(Player* player) override
    {
        player->GainHP(20);
    }
};