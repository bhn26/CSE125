#include "FieldObject.h"
#include "ObjectId.h"
#include "Entity.h"

FieldObject::FieldObject(const btVector3& origin,
                         btCollisionShape* fieldshape,
                         int team_id,
                         btDiscreteDynamicsWorld* curWorld)
    : m_curWorld(curWorld), m_teamId(team_id)
{
    m_fieldGhostObject = new btPairCachingGhostObject();
    m_fieldGhostObject->setCollisionShape(fieldshape);
    m_fieldGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    m_fieldGhostObject->setUserIndex(static_cast<int>(ClassId::Field));
    curWorld->addCollisionObject(m_fieldGhostObject,
                                 btBroadphaseProxy::SensorTrigger,
                                 btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
    m_fieldGhostObject->setWorldTransform(btTransform(btQuaternion::getIdentity(), origin));
}

FieldObject::~FieldObject()
{
}

int FieldObject::HandleField()
{
    return 1;
}