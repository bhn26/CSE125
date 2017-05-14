#pragma once
#include "PowerUp.h"
#include "Player.h"

class SpeedUp : public PowerUp
{
public:
    static const int m_speedBonus = 8;
    static const int m_speedBonusDuration = 400;

    SpeedUp() { m_type = PowerUpType::SpeedUp; }
    ~SpeedUp() {}
    // applies the powerup to the player
    void ApplyPower(Player* player) override
    {
        player->SetBonusSpeed(m_speedBonusDuration);
        player->SetPowerUpDuration(m_speedBonusDuration);
        player->EquipPower(this);
    }
    void RemovePower(Player* player) override
    {
        player->SetBonusSpeed(0);
        player->SetPowerUpDuration(0);
        player->ResetPower();
        delete this;
    }
};