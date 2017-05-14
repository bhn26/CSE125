#include "PlayerModifierCollision.h"

PlayerModifierCollision::PlayerModifierCollision(Player* m_shooter)
{
    m_shooter = m_shooter;
}

PlayerModifierCollision::~PlayerModifierCollision()
{
}

// Does not consider what collided with it
bool PlayerModifierCollision::HandleBulletCollision(unsigned int world_tick, Entity* collidee)
{
    if (!m_bullet)
    {
        return true;
    }

    // stay in bounds
    btVector3 bul_pos = m_bullet->GetEntityPosition();
    if (bul_pos.getX() > Constants::worldWidth - 5)
    {
        bul_pos.setX(Constants::worldWidth - 5);
    }
    else if (bul_pos.getX() < (Constants::worldWidth * (-1)) + 5)
    {
        bul_pos.setX((Constants::worldWidth * (-1)) + 5);
    }

    if (bul_pos.getZ() > Constants::worldWidth - 5)
    {
        bul_pos.setZ(Constants::worldWidth - 5);
    }
    else if (bul_pos.getZ() < (Constants::worldWidth * (-1)) + 5)
    {
        bul_pos.setZ((Constants::worldWidth * (-1)) + 5);
    }

    if (bul_pos.getY() < 2)
    {
        bul_pos.setY(2);
    }

    m_shooter->SetEntityPosition(&bul_pos);

    // This is pretty much for the teleport gun right now
    return true;
}