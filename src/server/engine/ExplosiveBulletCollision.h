#pragma once
#include "BulletCollisionHandler.h"
#include "Bullet.h"
// Explosive handler that explodes on contact
class ExplosiveBulletCollision : public BulletCollisionHandler
{
public:
    // the number of bounces the bullet should have
    ExplosiveBulletCollision(int size, int ttl) : m_ttl(ttl), m_explosionSize(size){};

    ~ExplosiveBulletCollision(){};

    // This needs to be called after the constructor because hack.
    void SetBullet(Bullet* b) { m_bullet = b; }

    // Does not consider what collided with it
    bool HandleBulletCollision(unsigned int world_tick, Entity* collidee) override;

private:
    int m_explosionSize = 0;  // size of the explosion
    int m_ttl = 0;            // duration of the explosion
    Bullet* m_bullet = nullptr; // the bullet that owns this collision handler
};