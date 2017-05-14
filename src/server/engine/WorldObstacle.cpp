
#include "WorldObstacle.h"

WorldObstacle::WorldObstacle(int objectid,
                             btRigidBody* rigidbody,
                             btDiscreteDynamicsWorld* curworld)
    : m_id(objectid), m_obstacleRigidBody(rigidbody), m_curWorld(curworld)
{
}

WorldObstacle::~WorldObstacle()
{
    // should we delete Rigid body here?  or elswhere...
    m_curWorld->removeCollisionObject(m_obstacleRigidBody);
    delete m_obstacleRigidBody->getMotionState();
    delete m_obstacleRigidBody->getCollisionShape();
    delete m_obstacleRigidBody;
}

int WorldObstacle::GetObjectId()
{
    return m_id;
}

btRigidBody* WorldObstacle::GetObjectRigidBody()
{
    return m_obstacleRigidBody;
}
