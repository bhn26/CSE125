#include "BlastField.h"
#include "Player.h"
#include "Entity.h"
#include "FireRateReset.h"

BlastField::BlastField(bool nonFriendly,
                       int m_stun,
                       int magnitude,
                       int ttl,
                       int damage,
                       btVector3* origin,
                       btCollisionShape* fieldshape,
                       int m_teamId,
                       btDiscreteDynamicsWorld* curworld)
    : FieldObject(origin, fieldshape, m_teamId, curworld)
    , m_stun(m_stun)
    , m_fieldTtl(ttl)
    , m_fieldDamage(damage)
    , m_nonFriendly(nonFriendly)
    , m_magnitude(magnitude)
{
}

BlastField::~BlastField()
{
    m_curWorld->removeCollisionObject(m_fieldGhostObject);
    delete m_fieldGhostObject->getCollisionShape();
    delete m_fieldGhostObject;
}

int BlastField::handleField()
{
    m_fieldTtl--;
    int numOverlap = m_fieldGhostObject->getNumOverlappingObjects();
    for (int i = 0; i < numOverlap; i++)
    {
        btRigidBody* pRigidBody =
            dynamic_cast<btRigidBody*>(m_fieldGhostObject->getOverlappingObject(i));
        if (pRigidBody && static_cast<ClassId>(pRigidBody->getUserIndex()) != ClassId::Obstacle
            && static_cast<ClassId>(pRigidBody->getUserIndex()) != ClassId::Field)
        {
            Entity* ent = (Entity*)pRigidBody->getUserPointer();

            if (ent->GetClassId() == ClassId::Player)
            {
                Player* collidedPlayer = (Player*)ent;
                if (collidedPlayer->GetTeamId() != m_teamId || m_nonFriendly)
                { // If the player is an enemy or if this field is nonfriendly
                    collidedPlayer->takeDamage(m_fieldDamage,
                                               FireRateReset::instance()->currentWorldTick);
                    collidedPlayer->SetStun(m_stun);
                }
                else
                {
                    break;
                }
            }

            // if we haven't already blasted this kid
            if (blasted.find(std::pair<ClassId, int>(ent->GetClassId(), ent->GetObjectId()))
                == blasted.end())
            {
                btTransform field = m_fieldGhostObject->getWorldTransform();
                btVector3 fieldCenter = field.getOrigin();

                btVector3 playerCenter = ent->GetEntityPosition();
                btVector3 blastDirection = playerCenter - fieldCenter;
                blastDirection = blastDirection.normalize();
                btVector3 velocity = m_magnitude * blastDirection;
                blastDirection.setY(ydirection);

                ent->Move(&velocity);
                std::pair<ClassId, int> entpair(ent->GetClassId(), ent->GetObjectId());
                blasted.emplace(entpair, 1);
            }
        }
    }

    if (m_fieldTtl > 0)
    {
        return 0;
    }
    return 1;
}