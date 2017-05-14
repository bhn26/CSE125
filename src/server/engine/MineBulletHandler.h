#pragma once
#include "BulletCollisionHandler.h"
#include "Entity.h"
#include "Bullet.h"
#include "FieldObject.h"

// Handler for stationary objects that do something when a player contacts it
class MineBulletHandler : public BulletCollisionHandler
{
private:
    Bullet* m_bullet = nullptr;     // the bullet that owns this collision handler
    FieldObject* m_field = nullptr; // the field that triggers upon colliding with this mine

public:
    // The field that triggers on this mine
    MineBulletHandler(FieldObject* field);

    // returns true when the bullet should be deleted, returns true on contact with player
    ~MineBulletHandler(){};

    // This needs to be called after the constructor because hack.
    void SetBullet(Bullet* bul) { m_bullet = bul; }

    bool HandleBulletCollision(unsigned int world_tick, Entity* collidee);
};