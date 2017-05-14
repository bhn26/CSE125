#pragma once
#include "BulletCollisionHandler.h"
#include "Bullet.h"

// simple collision handler, always signals the m_bullet for deletion on collision
class BounceBulletCollision : public BulletCollisionHandler
{
public:
    // the number of m_bounces the m_bullet should have
    BounceBulletCollision(int b, const btVector3& initialSpeed)
        : m_bounces(b), m_speed(initialSpeed){};

    // Does not consider what it collided with
    bool HandleBulletCollision(unsigned int world_tick, Entity* collidee)
    {
        if (m_bullet == nullptr)
        {
            return false;
        }

        if (--m_bounces == 0)
        {
            return true;
        }
        m_bullet->GetRigidBody()->setLinearVelocity(m_speed);

        return false;
    }

    // This needs to be called after the constructor because hack.
    void SetBullet(Bullet* bul) { m_bullet = bul; }

private:
    int m_bounces = 0;          // the number of m_bounces left before the m_bullet should expire
    Bullet* m_bullet = nullptr; // the m_bullet that owns this collision handler
    btVector3 m_speed = btVector3(0, 0, 0); // initial m_speed of the m_bullet
};