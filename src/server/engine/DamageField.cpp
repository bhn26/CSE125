#include "DamageField.h"
#include "Player.h"
#include "Entity.h"
#include "ObjectId.h"
#include "FireRateReset.h"

DamageField::DamageField(int ttl,
                         int damage,
                         const btVector3& origin,
                         btCollisionShape* fieldshape,
                         int team_id,
                         btDiscreteDynamicsWorld* curworld)
    : FieldObject(origin, fieldshape, team_id, curworld)
{
    m_fieldTtl = ttl;
    m_fieldDamage = damage;
}

DamageField::~DamageField()
{
    m_curWorld->removeCollisionObject(m_fieldGhostObject);
    delete m_fieldGhostObject->getCollisionShape();
    delete m_fieldGhostObject;
}

int DamageField::HandleField()
{
    m_fieldTtl--;
    int numOverlap = m_fieldGhostObject->getNumOverlappingObjects();
    for (int i = 0; i < numOverlap; i++)
    {
        btRigidBody* pRigidBody =
            dynamic_cast<btRigidBody*>(m_fieldGhostObject->getOverlappingObject(i));
        if (pRigidBody && static_cast<ClassId>(pRigidBody->getUserIndex()) == ClassId::Player)
        {
            Player* collidedPlayer = static_cast<Player*>(pRigidBody->getUserPointer());
            if (collidedPlayer->GetTeamId() != m_teamId)
            {
                if (collidedPlayer->takeDamage(m_fieldDamage,
                                               FireRateReset::Instance()->m_currentWorldTick))
                {
                }
            }
        }
    }

    if (m_fieldTtl > 0)
    {
        return 0;
    }
    return 1;
}