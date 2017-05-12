#pragma once
#include "PowerUp.h"
#include "Player.h"

class JumpUp : public PowerUp
{
public:
    static const int jump_bonus = 30;
    static const int jump_bonus_duration = 1200;

    JumpUp() { this->type = PowerUpType::JumpUp; }
    ~JumpUp() {}
    void applyPower(Player* p) override // applies the powerup to the player
    {
        p->SetBonusJump(jump_bonus);
        p->SetPowerUpDuration(jump_bonus_duration);
        p->EquipPower(this);
    };
    void removePower(Player* p) override
    {
        p->SetBonusJump(0);
        p->SetPowerUpDuration(0);
        p->ResetPower();
        delete this;
    };
};