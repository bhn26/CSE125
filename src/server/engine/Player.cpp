#include "EntitySpawner.h"
#include "ObjectId.h"

#include "server\ServerGame.h"

#include "Player.h"
#include "Flag.h"
#include "Peck.h"
#include "SeedGun.h"
#include "GrenadeLauncher.h"
#include "RespawnHandler.h"
#include <time.h>
#include "../ServerGame.h"

Player::Player(int objectid, int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
    : Entity(ClassId::Player, objectid, physicsWorld)
    , m_teamId(teamid)
    , m_position(pos)
    , m_peckWeapon(new Peck(m_curWorld))
{
    btCollisionShape* playerShape = new btSphereShape(1); // btCylinderShape(btVector3(1, 1, 1));
    // Create player physics object
    btDefaultMotionState* playerMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, 90, pos.z)));
    btScalar mass = 100;
    btVector3 playerInertia(0, 0, 0);
    playerShape->calculateLocalInertia(mass, playerInertia);
    btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(
        mass, playerMotionState, playerShape, playerInertia);
    btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
    // only allow rotation around Y-axis
    pRigidBody->forceActivationState(DISABLE_DEACTIVATION);
    pRigidBody->setDamping((btScalar)0.8, (btScalar)60.0);
    pRigidBody->setFriction((btScalar)0);
    physicsWorld->addRigidBody(pRigidBody);

    btTransform currentTrans;
    playerMotionState->getWorldTransform(currentTrans);
    btQuaternion currentOrientation = currentTrans.getRotation();
    m_cameraAngle = currentOrientation;

    // Set Player protected fields
    m_entityRigidBody = pRigidBody;

    // Set RigidBody to point to Player
    pRigidBody->setUserPointer(this);
    pRigidBody->setUserIndex(static_cast<int>(ClassId::Player));
}

Player::~Player()
{
    m_curWorld->removeCollisionObject(m_entityRigidBody);
    m_flags.clear();
    delete m_entityRigidBody->getMotionState();
    delete m_entityRigidBody->getCollisionShape();
    delete m_entityRigidBody;
}

void Player::PrintPlayerVelocity()
{
    // Calculate new velocity
    btTransform currentTrans;
    m_entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
    btMatrix3x3 currentOrientation = currentTrans.getBasis();
    // printf("current velocity %f, %f, %f\n", float(m_entityRigidBody->getLinearVelocity()[0]),
    // float(playerRigidBody->getLinearVelocity()[1]),
    // float(playerRigidBody->getLinearVelocity()[2]));
}

void Player::JumpPlayer()
{
    if (!m_alive)
    {
        return;
    }

    if (m_jumpSem)
    {
        // Change jump semaphore, change upward y-axis velocity
        m_jumpSem--;
        btVector3 curVelocity = m_entityRigidBody->getLinearVelocity();
        // setting upward velocity to 3
        curVelocity[1] = GetPlayerJump();
        m_entityRigidBody->setLinearVelocity(curVelocity);
    }
}

void Player::ResetJump()
{
    (m_jumpSem) = 2;
}

void Player::AcquireFlag(Flag* flag)
{
    if (!m_alive)
        return;
    // player collects flag, remove from entity list
    m_flags.push_back(flag);
    switch (m_flags.size())
    {
    case 1:
        m_speedPenalty = 1;
        break;
    case 2:
        m_speedPenalty = 3;
        break;
    case 3:
        m_speedPenalty = 6;
        break;
    case 4:
        m_speedPenalty = 10;
        break;
    case 5:
        m_speedPenalty = 15;
        break;
    default:
        m_speedPenalty = 15;
        break;
    }
    printf("Flag ACQUIRED\n");

    // note - individual scores are updated with move packets
    ServerGame::instance()->IncScore(m_teamId, 1);
    // ServerGame::instance()->sendScorePacket();

    // check if your team won
    int* scores = ServerGame::instance()->GetScores();
    printf("Team %d has %d out of %d eggs\n",
           m_teamId,
           scores[m_teamId],
           ServerGame::instance()->GetTotalEggs());
    if (scores[m_teamId] == ServerGame::instance()->GetTotalEggs())
    {
        printf("sending game over packet\n");
        ServerGame::instance()->sendGameOverPacket(m_teamId);
    }
}

void Player::LoseFlags()
{
    ServerGame::instance()->DecScore(m_teamId, m_flags.size());

    // Change this, we need the flags to come out of the player back into the world
    m_flags.clear();
    // ServerGame::instance()->sendScorePacket();
}

int Player::GetTeamId()
{
    return m_teamId;
}

void Player::UseWeapon()
{
    if (!m_alive)
        return;

    if (m_stunCount > 0)
        return;

    // printf("Player %u : attempting to use weapon\n", m_objectId);
    // If player weapon doesn't exist, exit
    if (!m_playerWeapon)
    {
        return;
    }

    // passes player position when using weapon
    btVector3 temp = GetEntityPosition();

    btQuaternion* playerRotation = &(m_cameraAngle);

    btVector3* position = new btVector3(temp.getX(), temp.getY(), temp.getZ());

    btMatrix3x3* currentOrientation = new btMatrix3x3(*playerRotation);

    // ServerGame::instance()->sendShootPacket(m_objectId);
    if (m_playerWeapon->UseWeapon(position, currentOrientation, m_objectId, m_teamId, this)
        == 0)
    {
        ServerGame::instance()->sendDiscardPacket(GetObjectId());
        delete m_playerWeapon;
        m_playerWeapon = nullptr;
    }
    ServerGame::instance()->sendAttackPacket(m_objectId);
}

void Player::SetCamAngle(float yos)
{
    btQuaternion playerRotation = GetEntityRotation();
    if (yos > -9990)
    {
        yos += .2;
        printf("Camera Vertical Angle is: %f\n", yos);
        m_cameraAngle = (playerRotation) * (btQuaternion(btVector3(-1, 0, 0), yos));
    }
}

void Player::EquipWeapon(Weapon* newWeapon)
{
    if (!m_alive)
        return;
    m_playerWeapon = newWeapon;
}

void Player::EquipPower(PowerUp* powerup)
{
    if (!m_alive)
        return;
    power = powerup;
}

void Player::LosePower()
{
    if (power)
    {
        delete power;
        power = nullptr;
    }
}

void Player::DiscardWeapon()
{
    if (m_playerWeapon)
    {
        delete m_playerWeapon;
        m_playerWeapon = nullptr;
        ServerGame::instance()->sendDiscardPacket(m_objectId);
    }

    // if(m_alive) send packet for discard animation
}

bool Player::HasWeapon() const
{
    return m_playerWeapon != nullptr;
}

bool Player::HasPower()
{
    return power != nullptr;
}

// If player is dead, returns 1,  else returns 0
int Player::takeDamage(int damage, unsigned int world_tick)
{
    if (!m_alive)
    {
        return 0;
    }

    m_hitPoints = m_hitPoints - damage;

    if (m_hitPoints <= 0)
    {
        HandleDeath(world_tick);
        return 1;
    }
    else
    {
        return 0;
    }
}

void Player::UsePeck()
{
    if (!m_alive)
        return;

    if (m_stunCount > 0)
        return;

    btVector3 temp = GetEntityPosition();

    btQuaternion* playerRotation = &(GetEntityRotation());
    // playerRotation->setX(position.camx);
    // playerRotation->setZ(position.camz);

    btVector3* position = new btVector3(temp.getX(), temp.getY(), temp.getZ());

    btTransform currentTrans;
    m_entityRigidBody->getMotionState()->getWorldTransform(currentTrans);
    btMatrix3x3* currentOrientation = new btMatrix3x3(*playerRotation);

    m_peckWeapon->UseWeapon(position, currentOrientation, m_objectId, m_teamId, this);
    ServerGame::instance()->sendAttackPacket(m_objectId);
    //	peckWeapon->UseWeapon(&(m_entityRigidBody->getCenterOfMassPosition()), &currentOrientation,
    //m_objectId, m_teamId, this);
    // printf("player with objId: %d used peck! \n", m_objectId);
}

void Player::HandleDeath(unsigned int death_tick)
{
    // printf("Player %u has died!", m_objectId);
    m_alive = false;
    m_speedPenalty = 0;
    DiscardWeapon();
    LosePower();
    m_deathTime = death_tick;
    ServerGame::instance()->sendDeathPacket(m_objectId);
    // EntitySpawner::instance()->RemoveEntity(classId, m_objectId);

    RespawnHandler::instance()->KillPlayer(this); // Schedule this guy to respawn in the future

    btVector3 deathPos = m_entityRigidBody->getCenterOfMassPosition();
    deathPos.setY((deathPos.getY() + 4));
    // srand(time(NULL));
    btTransform currentTrans;
    btVector3 ranVelocity;

    // Disperse the player's flags
    for (auto it = m_flags.begin(); it != m_flags.end();)
    {
        Flag* curFlag = (*it);
        curFlag->GetRigidBody()->getMotionState()->getWorldTransform(currentTrans);
        currentTrans.setOrigin(deathPos);
        curFlag->GetRigidBody()->getMotionState()->setWorldTransform(currentTrans);
        curFlag->GetRigidBody()->setCenterOfMassTransform(currentTrans);
        deathPos.setY((deathPos.getY() + 4));
        std::pair<int, int> vel = EntitySpawner::getRandomLoc();
        ranVelocity = btVector3((vel.first % 100), (rand() % 100), (vel.second % 100));

        // add Flag to world
        m_curWorld->addRigidBody(curFlag->GetRigidBody());
        curFlag->GetRigidBody()->setLinearVelocity(ranVelocity);
        // add Flag to client
        btQuaternion quat = curFlag->GetEntityRotation();
        PosInfo out;
        out.cid = ClassId::Flag;
        out.oid = curFlag->GetObjectId();
        out.x = deathPos.getX();
        out.y = deathPos.getY();
        out.z = deathPos.getZ();
        out.rotw = quat.getW();
        out.rotx = quat.getX();
        out.roty = quat.getY();
        out.rotz = quat.getZ();
        // add Flag to Entity Map
        ServerGame::instance()->sendSpawnPacket(out);
        EntitySpawner::instance()->AddEntity(
            curFlag->GetClassId(), curFlag->GetObjectId(), curFlag);

        // erase flag from player
        it = m_flags.erase(it);
        ServerGame::instance()->DecScore(m_teamId, 1);
    }
    // m_flags.clear(); //Actually calls delete on flags... didn't seem to correctly work for bullet
    // deletion anways... maybe cause of void pointer
    // LoseFlags();
    // ServerGame::instance()->sendScorePacket();
}

void Player::Move(btVector3* changeVelocity)
{
    if (!m_alive)
        return;

    Entity::Move(changeVelocity);
}
/*
void Player::SetEntityRotation(float x, float y, float z, float w)
{
    if (!m_alive)
        return;
    Entity::SetEntityRotation(x, y, z, w);
}*/
