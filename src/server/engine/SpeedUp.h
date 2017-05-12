#pragma once
#include "PowerUp.h"
#include "Player.h"

class SpeedUp : public PowerUp
{
public:
    static const int speed_bonus = 8;
    static const int speed_bonusDuration = 400;

    SpeedUp() { this->type = PowerUpType::SpeedUp; }
    ~SpeedUp() {}
    // applies the powerup to the player
    void applyPower(Player* p) override
    {
        p->SetBonusSpeed(speed_bonusDuration);
        p->SetPowerUpDuration(speed_bonusDuration);
        p->EquipPower(this);
    };
    void removePower(Player* p) override
    {
        p->SetBonusSpeed(0);
        p->SetPowerUpDuration(0);
        p->ResetPower();
        delete this;
    };
};