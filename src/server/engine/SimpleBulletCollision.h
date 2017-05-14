#pragma once
#include "BulletCollisionHandler.h"

// simple collision handler, always signals the bullet for deletion on collision
class SimpleBulletCollision : public BulletCollisionHandler
{
public:
    SimpleBulletCollision(){};
    ~SimpleBulletCollision(){};

    // Does not consider what collided with it
    bool HandleBulletCollision(unsigned int world_tick, Entity* collidee) override { return true; }
};
