#pragma once
#include "PowerUp.h"
#include "Player.h"

class JumpUp : public PowerUp
{
public:
    static const int s_jumpBonus = 30;
    static const int s_jumpBonusDuration = 1200;

    JumpUp() { m_type = PowerUpType::JumpUp; }
    ~JumpUp() {}
    void ApplyPower(Player* player) override // applies the powerup to the player
    {
        player->SetBonusJump(s_jumpBonus);
        player->SetPowerUpDuration(s_jumpBonusDuration);
        player->EquipPower(this);
    }
    void RemovePower(Player* player) override
    {
        player->SetBonusJump(0);
        player->SetPowerUpDuration(0);
        player->ResetPower();
        delete this;
    }
};