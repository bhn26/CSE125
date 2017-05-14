#include "ExplosiveBulletCollision.h"
#include "DamageField.h"
#include "FieldHandler.h"

// Does not consider what collided with it
bool ExplosiveBulletCollision::HandleBulletCollision(unsigned int world_tick, Entity* collidee)
{
    if (!m_bullet)
    {
        return false;
    }

    btVector3 temp = m_bullet->GetEntityPosition();
    btVector3 position = btVector3(temp.getX(), temp.getY(), temp.getZ());

    btCollisionShape* expSphere = new btSphereShape(btScalar(m_explosionSize));
    DamageField* explosionField = new DamageField(m_ttl,
                                                  m_bullet->GetDamage(),
                                                  position,
                                                  expSphere,
                                                  m_bullet->GetTeamId(),
                                                  m_bullet->GetPhysicsWorld());

    // TODO: Add to damage field checker. Add this new field to the checker
    FieldHandler::Instance()->AddField(explosionField);

    return true;
}