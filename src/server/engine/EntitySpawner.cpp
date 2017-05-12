#include "EntitySpawner.h"
#include "Entity.h"
#include "Bullet.h"
#include "Player.h"
#include "Flag.h"
#include "../ServerGame.h"

// Collectable weapons
#include "Collectable.h"
#include "CollectableSpawner.h"
#include "SeedGun.h"
#include "BounceGun.h"
#include "GrenadeLauncher.h"
#include "TeleportGun.h"
#include "BlastMine.h"
#include "Shotgun.h"

// Collectabable powerups
#include "HealthGain.h"
#include "JumpUp.h"
#include "SpeedUp.h"

EntitySpawner* EntitySpawner::spawnInstance = nullptr;

EntitySpawner* EntitySpawner::instance()
{
    if (!spawnInstance)
    {
        spawnInstance = new EntitySpawner();
    }
    return spawnInstance;
}

EntitySpawner::EntitySpawner()
{
    oid_player = 0;
    oid_flag = 0;
    oid_bullet = 0;
    oid_collectable = 0;
}

EntitySpawner::~EntitySpawner()
{
}

Player* EntitySpawner::spawnPlayer(int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
{
    // Create player and add to Entity Map
    // note - need to create players with explicit id or else teams will depend on order they
    // connected to the lobby
    Player* newPlayer = new Player(pos.id, pos.team_id, pos, physicsWorld);
    AddEntity(ClassId::Player, pos.id, newPlayer);
    oid_player++;

    btQuaternion quat = newPlayer->GetEntityRotation();

    // Send Player Spawn packet
    btVector3 vec = newPlayer->GetEntityPosition();
    printf("Created player %d at (%f,%f,%f) on team %d\n",
           pos.id,
           vec.getX(),
           vec.getY(),
           vec.getZ(),
           pos.team_id);
    // Send spawn info to the clients
    PosInfo out;
    out.cid = ClassId::Player;
    out.oid = newPlayer->GetObjectId();
    out.skin = pos.skin;
    out.team_id = pos.team_id;
    out.x = vec.getX();
    out.y = vec.getY();
    out.z = vec.getZ();
    out.rotw = quat.getW();
    out.rotx = quat.getX();
    out.roty = quat.getY();
    out.rotz = quat.getZ();

    ServerGame::instance()->sendSpawnPacket(out);
    return newPlayer;
}

Flag* EntitySpawner::spawnFlag(PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
{
    // Create flag and add to Entity Map
    Flag* newFlag = new Flag(oid_flag, pos, physicsWorld);
    AddEntity(ClassId::Flag, oid_flag, newFlag);
    oid_flag++;

    // Send Flag Spawn packet
    btVector3 vec = newFlag->GetEntityPosition();
    btQuaternion quat = newFlag->GetEntityRotation();

    PosInfo out;
    out.cid = ClassId::Flag;
    out.oid = newFlag->GetObjectId();
    out.x = vec.getX();
    out.y = vec.getY();
    out.z = vec.getZ();
    out.rotw = quat.getW();
    out.rotx = quat.getX();
    out.roty = quat.getY();
    out.rotz = quat.getZ();
    ServerGame::instance()->sendSpawnPacket(out);
    return newFlag;
}

Bullet* EntitySpawner::spawnBullet(int playerid,
                                   int teamid,
                                   int damage,
                                   WeaponType shooter,
                                   BulletCollisionHandler* handler,
                                   btRigidBody* bullet_body,
                                   btDiscreteDynamicsWorld* physicsWorld)
{
    // Create Bullet and add to Entity Map
    // Bullet* fireProjectile = new Bullet(oid_bullet, playerid, teamid, damage, pos, velocity,
    // rotation, physicsWorld);
    Bullet* fireProjectile =
        new Bullet(oid_bullet, playerid, teamid, damage, handler, bullet_body, physicsWorld);
    AddEntity(ClassId::Bullet, oid_bullet, fireProjectile);
    oid_bullet++;

    // Send Bullet Spawn packet
    btVector3 vec = fireProjectile->GetEntityPosition();
    btQuaternion quat = fireProjectile->GetEntityRotation();
    // printf("Created Bullet at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());

    PosInfo out;
    out.cid = ClassId::Bullet;
    out.oid = fireProjectile->GetObjectId();
    out.x = vec.getX();
    out.y = vec.getY();
    out.z = vec.getZ();
    out.sub_id = static_cast<int>(shooter);
    out.rotw = quat.getW();
    out.rotx = quat.getX();
    out.roty = quat.getY();
    out.rotz = quat.getZ();
    ServerGame::instance()->sendSpawnPacket(out);
    return fireProjectile;
}

void EntitySpawner::spawnCollectable(btDiscreteDynamicsWorld* curWorld, WeaponType w_type)
{
    Weapon* wp;
    // switch (w_type)
    switch (w_type)
    {
    case WeaponType::SeedGun:
        printf("spawned seedgun\n");
        wp = new SeedGun(curWorld);
        break;
    case WeaponType::BounceGun:
        printf("spawned bouncegun\n");
        wp = new BounceGun(curWorld);
        break;
    case WeaponType::GrenadeLauncher:
        printf("spawned grenadelauncher\n");
        wp = new GrenadeLauncher(curWorld);
        break;
    case WeaponType::TeleportGun:
        printf("spawned teleportgun\n");
        wp = new TeleportGun(curWorld);
        break;
    case WeaponType::BlastMine:
        printf("spawned blastmine\n");
        wp = new BlastMine(curWorld);
        break;
    case WeaponType::Shotgun:
        printf("spawned shotgun\n");
        wp = new Shotgun(curWorld);
        break;
    default:
        wp = nullptr;
        break;
    }

    if (!wp)
    {
        return;
    }

    std::pair<int, int> p = getRandomLoc();
    PosInfo pos;
    pos.x = p.first;
    pos.y = 90;
    pos.z = p.second;

    Collectable* ranCollectable = new Collectable(oid_collectable, pos, curWorld, wp);
    AddEntity(ClassId::Collectable, oid_collectable, ranCollectable);
    oid_collectable++;

    // Send Collectable Spawn packet
    btVector3 vec = ranCollectable->GetEntityPosition();
    btQuaternion quat = ranCollectable->GetEntityRotation();

    PosInfo out;
    out.cid = ClassId::Collectable;
    out.oid = ranCollectable->GetObjectId();
    out.x = vec.getX();
    out.y = vec.getY();
    out.z = vec.getZ();
    out.sub_id = static_cast<int>(CollectType::Weapon);
    out.rotw = quat.getW();
    out.rotx = quat.getX();
    out.roty = quat.getY();
    out.rotz = quat.getZ();
    ServerGame::instance()->sendSpawnPacket(out);
}

void EntitySpawner::spawnCollectable(btDiscreteDynamicsWorld* curWorld, PowerUpType p_type)
{
    PowerUp* pow = nullptr;

    switch (p_type)
    {
    case PowerUpType::HealthGain:
        printf("spawned healthgain\n");
        pow = new HealthGain();
        break;
    case PowerUpType::JumpUp:
        printf("spawned jumpup\n");
        pow = new JumpUp();
        break;
    /*case PowerUpType::SpeedUp:
        printf("spawned speedup\n");
        pow = new SpeedUp();
        break;*/
    default:
        pow = nullptr;
        break;
    }

    if (pow == nullptr)
        return;

    std::pair<int, int> p = getRandomLoc();
    PosInfo pos;
    pos.x = p.first;
    pos.y = 90;
    pos.z = p.second;

    Collectable* ranCollectable = new Collectable(oid_collectable, pos, curWorld, pow);
    AddEntity(ClassId::Collectable, oid_collectable, ranCollectable);
    oid_collectable++;

    // Send Collectable Spawn packet
    btVector3 vec = ranCollectable->GetEntityPosition();
    btQuaternion quat = ranCollectable->GetEntityRotation();

    PosInfo out;
    out.cid = ClassId::Collectable;
    out.oid = ranCollectable->GetObjectId();
    out.x = vec.getX();
    out.y = vec.getY();
    out.z = vec.getZ();
    out.sub_id = static_cast<int>(CollectType::PowerUp);
    out.rotw = quat.getW();
    out.rotx = quat.getX();
    out.roty = quat.getY();
    out.rotz = quat.getZ();
    ServerGame::instance()->sendSpawnPacket(out);
}

void EntitySpawner::AddEntity(ClassId cid, unsigned int oid, Entity* ent)
{
    std::pair<ClassId, unsigned int> p(cid, oid);
    // entities[p] = std::move(ent);
    m_entities[p] = ent;
}

Entity* EntitySpawner::GetEntity(ClassId cid, unsigned int oid)
{
    std::pair<ClassId, unsigned int> key(cid, oid);
    return m_entities[key];
}

void EntitySpawner::RemoveEntity(ClassId cid, unsigned int oid)
{
    std::pair<ClassId, unsigned int> key(cid, oid);
    auto it = m_entities.find(key);
    if (it != m_entities.end())
    {
        m_entities.erase(it);
        // printf("? supposedly removed entity from map\n");
    }
}

std::map<std::pair<ClassId, unsigned int>, Entity*>& EntitySpawner::GetMap()
{
    return m_entities;
}

std::pair<int, int> EntitySpawner::getRandomLoc()
{
    std::pair<int, int> loc;
    loc.first = 0;
    loc.second = 0;
    int random = rand() % 4;
    switch (random)
    {
    case 0:
        loc.first = (rand() % Constants::worldWidth + 1);
        loc.second = (rand() % Constants::worldWidth + 1);
        break;
    case 1:
        loc.first = (rand() % Constants::worldWidth + 1);
        loc.second = (-1 * rand() % Constants::worldWidth + 1);
        break;
    case 2:
        loc.first = (-1 * rand() % Constants::worldWidth + 1);
        loc.second = (rand() % Constants::worldWidth + 1);
        break;
    case 3:
        loc.first = (-1 * rand() % Constants::worldWidth + 1);
        loc.second = (-1 * rand() % Constants::worldWidth + 1);
        break;
    }
    return loc;
}
