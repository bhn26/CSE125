#pragma once
#include "BulletCollisionHandler.h"
#include "Player.h"
#include "Bullet.h"

// This class is for any bullets that modify the shooter
class PlayerModifierCollision : public BulletCollisionHandler
{
private:
    Player* m_shooter = nullptr;
    Bullet* m_bullet = nullptr;

public:
    PlayerModifierCollision(Player* shooter);
    ~PlayerModifierCollision();

    // This needs to be called after the constructor because hack.
    void SetBullet(Bullet* b) { m_bullet = b; }

    // Does not consider what collided with it
    bool HandleBulletCollision(unsigned int world_tick, Entity* collidee);
};