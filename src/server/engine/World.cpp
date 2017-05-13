#include "World.h"
#include "ObjectId.h"
#include "../ServerGame.h"
#include "FireRateReset.h"
#include "FieldHandler.h"
#include "RespawnHandler.h"
#include "CollectableSpawner.h"
#include "Player.h"
#include "Flag.h"
#include "Bullet.h"
#include "WorldObstacle.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "MapLoader.h"

World::World()
{
    // initialize map objects
}

World::~World()
{
    // TODO handle vector deletes...
    delete curWorld;
}

void World::Init()
{
    int z = 1000; // this is a random number for the walls right now, we need to change this

    // Create Physics world
    btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    btDiscreteDynamicsWorld* dynamicsWorld =
        new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
    dynamicsWorld->setGravity(btVector3(0, -20, 0));

    // Add Ground Object
    // btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(1.),
    // btScalar(0.)), 0);
    btCollisionShape* groundShape =
        new btBoxShape(btVector3(Constants::worldWidth + 50, 1, Constants::worldWidth + 50));
    btDefaultMotionState* groundMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
        0, groundMotionState, groundShape, btVector3(0, 0, 0));
    groundRigidBodyCI.m_friction = 1.0;
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    groundRigidBody->setGravity(btVector3(0, 0, 0));
    dynamicsWorld->addRigidBody(groundRigidBody);
    groundRigidBody->setGravity(btVector3(0, 0, 0));
    groundRigidBody->setUserIndex(static_cast<int>(ClassId::Obstacle));
    // Create Ground Obstacle
    WorldObstacle* groundwall = new WorldObstacle(z++, groundRigidBody, curWorld);

    // Add Sky Object
    btCollisionShape* skyShape =
        new btBoxShape(btVector3(Constants::worldWidth + 50, 1, Constants::worldWidth + 50));
    btDefaultMotionState* skyMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, Constants::worldHeight, 0)));
    btRigidBody::btRigidBodyConstructionInfo skyRigidBodyCI(
        0, skyMotionState, skyShape, btVector3(0, 0, 0));
    skyRigidBodyCI.m_friction = .4;
    btRigidBody* skyRigidBody = new btRigidBody(skyRigidBodyCI);
    skyRigidBody->setGravity(btVector3(0, 0, 0));
    dynamicsWorld->addRigidBody(skyRigidBody);
    skyRigidBody->setUserIndex(static_cast<int>(ClassId::Obstacle));
    // Create Ground Obstacle
    WorldObstacle* skywall = new WorldObstacle(z++, skyRigidBody, curWorld);

    // Add Pos X Wall
    // btCollisionShape* xWallShape = new btStaticPlaneShape(btVector3(btScalar(-1.), btScalar(0.),
    // btScalar(0.)), 0);
    btCollisionShape* xWallShape =
        new btBoxShape(btVector3(1, Constants::worldHeight, Constants::worldWidth));
    btDefaultMotionState* xWallMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(Constants::worldWidth + 1, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo xWallRigidBodyCI(
        0, xWallMotionState, xWallShape, btVector3(0, 0, 0));
    xWallRigidBodyCI.m_friction = .5;
    btRigidBody* xWallRigidBody = new btRigidBody(xWallRigidBodyCI);
    xWallRigidBody->setGravity(btVector3(0, 0, 0));
    dynamicsWorld->addRigidBody(xWallRigidBody);
    xWallRigidBody->setUserIndex(static_cast<int>(ClassId::Obstacle));
    // Create X Wall
    WorldObstacle* xwall = new WorldObstacle(z++, xWallRigidBody, curWorld);

    // Add Neg X Wall
    // btCollisionShape* nxWallShape = new btStaticPlaneShape(btVector3(btScalar(1.), btScalar(0.),
    // btScalar(0.)), 0);
    btCollisionShape* nxWallShape =
        new btBoxShape(btVector3(1, Constants::worldHeight, Constants::worldWidth));
    btDefaultMotionState* nxWallMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(-(Constants::worldWidth + 1), 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo nxWallRigidBodyCI(
        0, nxWallMotionState, nxWallShape, btVector3(0, 0, 0));
    nxWallRigidBodyCI.m_friction = .5;
    btRigidBody* nxWallRigidBody = new btRigidBody(nxWallRigidBodyCI);
    nxWallRigidBody->setGravity(btVector3(0, 0, 0));
    dynamicsWorld->addRigidBody(nxWallRigidBody);
    nxWallRigidBody->setUserIndex(static_cast<int>(ClassId::Obstacle));
    // Create Neg X Wall
    WorldObstacle* nxwall = new WorldObstacle(z++, groundRigidBody, curWorld);

    // Add Pos Z Wall
    // btCollisionShape* zWallShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(0.),
    // btScalar(-1.)), 0);
    btCollisionShape* zWallShape =
        new btBoxShape(btVector3(Constants::worldWidth, Constants::worldHeight, 1));
    btDefaultMotionState* zWallMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, Constants::worldWidth + 1)));
    btRigidBody::btRigidBodyConstructionInfo zWallRigidBodyCI(
        0, zWallMotionState, zWallShape, btVector3(0, 0, 0));
    zWallRigidBodyCI.m_friction = .5;
    btRigidBody* zWallRigidBody = new btRigidBody(zWallRigidBodyCI);
    zWallRigidBody->setGravity(btVector3(0, 0, 0));
    dynamicsWorld->addRigidBody(zWallRigidBody);
    zWallRigidBody->setUserIndex(static_cast<int>(ClassId::Obstacle));
    // Create Pos Z Wall
    WorldObstacle* zwall = new WorldObstacle(z++, zWallRigidBody, curWorld);

    // Add Neg Z Wall
    // btCollisionShape* nzWallShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(0.),
    // btScalar(1.)), 0);
    btCollisionShape* nzWallShape =
        new btBoxShape(btVector3(Constants::worldWidth, Constants::worldHeight, 1));
    btDefaultMotionState* nzWallMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -(Constants::worldWidth + 1))));
    btRigidBody::btRigidBodyConstructionInfo nzWallRigidBodyCI(
        0, nzWallMotionState, zWallShape, btVector3(0, 0, 0));
    nzWallRigidBodyCI.m_friction = .5;
    btRigidBody* nzWallRigidBody = new btRigidBody(nzWallRigidBodyCI);
    nzWallRigidBody->setGravity(btVector3(0, 0, 0));
    dynamicsWorld->addRigidBody(nzWallRigidBody);
    nzWallRigidBody->setUserIndex(static_cast<int>(ClassId::Obstacle));
    // Create Neg Z Wall
    WorldObstacle* nzwall = new WorldObstacle(z++, nzWallRigidBody, curWorld);

    // Load in world objects
    this->worldMapLoader = new MapLoader(dynamicsWorld);
    worldMapLoader->loadMap();

    // Set Local attributes
    this->ground = groundwall;
    this->frontWall = nzwall;
    this->backWall = zwall;
    this->leftWall = nxwall;
    this->rightWall = xwall;

    curWorld = dynamicsWorld;
    solv = solver;
    pairCache = overlappingPairCache;
    disp = dispatcher;
    colConfig = collisionConfig;

    // set up physics world for field detection
    dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(
        new btGhostPairCallback());
}

btDiscreteDynamicsWorld* World::GetPhysicsWorld()
{
    return curWorld;
}

void World::PreSpawn()
{
    ServerGame::Instance()->sendReadyToSpawnPacket();
}

void World::UpdateWorld()
{
    // Step simulation
    curWorld->stepSimulation(1 / 60.f, 4); // 60
    //    curWorld->stepSimulation(1 / 30.f, 4);   // 30 frames

    FireRateReset::Instance()->currentWorldTick++;

    // Process Weapon Fire Rate Reset and field collisions
    FireRateReset::Instance()->ResetWeapons();
    FieldHandler::Instance()->HandleFields();

    // Process all collisions
    int numManifolds = curWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        // printf("numMan  %d\n", numManifolds);
        btPersistentManifold* contactManifold =
            curWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        // Handle Bullet Collisions--------------------------
        // If object A of collision is a Bullet
        if (static_cast<ClassId>(obA->getUserIndex()) == ClassId::Bullet)
        {
            // Grab Bullet Object
            Bullet* collideBullet = (Bullet*)obA->getUserPointer();
            if (collideBullet->GetMarked())
            {
                continue;
            }
            // If it hit a player
            if (static_cast<ClassId>(obB->getUserIndex()) == ClassId::Player)
            {
                Player* collidePlayer = (Player*)obB->getUserPointer();
                // ignore friendly fire
                if (collideBullet->GetTeamId() == collidePlayer->GetTeamId())
                {
                    continue;
                }
                // printf("Pushed to delete!, hit playerB");
                collideBullet->SetToMarked(world_tick);
                if (collideBullet->handleBulletCollision(world_tick, collidePlayer))
                {
                    deleteList.push_back(collideBullet);
                    ServerGame::Instance()->sendRemovePacket(ClassId::Bullet,
                                                             collideBullet->GetObjectId());
                }
                else
                {
                    unmarkList.push_back(collideBullet);
                }
                // TODO send "you got hit"
                if (collidePlayer->takeDamage(collideBullet->GetDamage(), world_tick))
                {
                    // printf("Player is dead!");
                    // TODO Handle Player death:  send player death to client
                }
            }

            // If it hit a bullet, ignore and just bounce off
            else if (static_cast<ClassId>(obB->getUserIndex()) == ClassId::Bullet
                     || static_cast<ClassId>(obB->getUserIndex()) == ClassId::Field)
            {
                continue;
            }
            else
            { // Collided with unknown object
                // deletes bulletA regardless
                // printf("Pushed to delete!, hit ground B,  %d", obB->getUserIndex());
                btVector3 bulPos = collideBullet->GetEntityPosition();
                // printf("Current position:  x: %f, y: %f, z: %f  \n", bulPos.getX(),
                // bulPos.getY(), bulPos.getZ());
                bulPos = collideBullet->GetRigidBody()->getLinearVelocity();
                // printf("Current velocity:  x: %f, y: %f, z: %f  \n", bulPos.getX(),
                // bulPos.getY(), bulPos.getZ());
                if (collideBullet->GetMarked())
                {
                    continue;
                }
                collideBullet->SetToMarked(world_tick);
                if (collideBullet->handleBulletCollision(world_tick, nullptr))
                {
                    deleteList.push_back(collideBullet);
                    ServerGame::Instance()->sendRemovePacket(ClassId::Bullet,
                                                             collideBullet->GetObjectId());
                }
                else
                {
                    unmarkList.push_back(collideBullet);
                }
            }
        }

        // If object B of collision is a Bullet
        else if (static_cast<ClassId>(obB->getUserIndex()) == ClassId::Bullet)
        {
            // Grab Bullet Object
            Bullet* collideBullet = (Bullet*)obB->getUserPointer();
            if (collideBullet->GetMarked())
            {
                continue;
            }
            // Bullet hits player
            if (static_cast<ClassId>(obA->getUserIndex()) == ClassId::Player)
            {
                Player* collidePlayer = (Player*)obA->getUserPointer();
                // ignore team fire
                if (collideBullet->GetTeamId() == collidePlayer->GetTeamId())
                {
                    continue;
                }

                collideBullet->SetToMarked(world_tick);
                if (collideBullet->handleBulletCollision(world_tick, collidePlayer))
                {
                    deleteList.push_back(collideBullet);
                    ServerGame::Instance()->sendRemovePacket(ClassId::Bullet,
                                                             collideBullet->GetObjectId());
                }
                else
                {
                    unmarkList.push_back(collideBullet);
                }
                if (collidePlayer->takeDamage(collideBullet->GetDamage(), world_tick))
                {
                    // printf("Player is dead!");
                    // TODO Handle Player death:  send player death to client
                }
            }

            // If it hit a bullet, ignore and bounce off
            else if (static_cast<ClassId>(obA->getUserIndex()) == ClassId::Bullet
                     || static_cast<ClassId>(obB->getUserIndex()) == ClassId::Field)
            {
                continue;
            }
            else // Collide with unknown object
            {
                if (collideBullet->GetMarked())
                {
                    continue;
                }
                // deletes bulletB regardless
                // printf("Pushed to delete!, hit ground A,  %d", obA->getUserIndex());
                btVector3 bulPos = collideBullet->GetEntityPosition();
                // printf("Current position:  x: %f, y: %f, z: %f  \n", bulPos.getX(),
                // bulPos.getY(), bulPos.getZ());
                bulPos = collideBullet->GetRigidBody()->getLinearVelocity();
                // printf("Current velocity:  x: %f, y: %f, z: %f  \n", bulPos.getX(),
                // bulPos.getY(), bulPos.getZ());
                collideBullet->SetToMarked(world_tick);
                if (collideBullet->handleBulletCollision(world_tick, nullptr))
                {
                    deleteList.push_back(collideBullet);
                    ServerGame::Instance()->sendRemovePacket(ClassId::Bullet,
                                                             collideBullet->GetObjectId());
                }
                else
                {
                    unmarkList.push_back(collideBullet);
                }
            }
        }

        // Handle Player Collisions -------------------------------------------------
        // Obj A is Player
        if (static_cast<ClassId>(obA->getUserIndex()) == ClassId::Player)
        {
            // Grab Player Object
            Player* collidePlayer = (Player*)obA->getUserPointer();

            // TODO, if Obj B is collectable, HandleCollectable();
            if (static_cast<ClassId>(obB->getUserIndex()) == ClassId::Collectable)
            {
                // Ignore dead guy
                if (!collidePlayer->IsAlive())
                    continue;

                // Handle Collectable Collection
                Collectable* collectObj = (Collectable*)obB->getUserPointer();
                // check if collectable has been handled already
                if (collectObj->GetMarked())
                {
                    continue;
                }
                collectObj->HandleCollect(collidePlayer);
                if (collectObj->GetType() == CollectType::Weapon)
                {
                    ServerGame::Instance()->sendRemovePacket(
                        ClassId::Collectable,
                        collectObj->GetObjectId(),
                        ClassId::Player,
                        collidePlayer->GetObjectId(),
                        CollectType::Weapon,
                        static_cast<int>(collectObj->GetWeapon()->GetWeaponType()));
                }
                else if (collectObj->GetType() == CollectType::PowerUp)
                {
                    ServerGame::Instance()->sendRemovePacket(
                        ClassId::Collectable,
                        collectObj->GetObjectId(),
                        ClassId::Player,
                        collidePlayer->GetObjectId(),
                        CollectType::PowerUp,
                        static_cast<int>(collectObj->GetPowerUp()->getType()));
                }
                deleteList.push_back(collectObj);
                collectObj->SetToMarked(world_tick);
            }

            // if Obj B is Flag
            else if (static_cast<ClassId>(obB->getUserIndex()) == ClassId::Flag)
            {
                // Ignore dead guy
                if (!collidePlayer->IsAlive())
                    continue;

                // Handle Flag Collection
                Flag* collideFlag = (Flag*)obB->getUserPointer();
                if (collideFlag->GetMarked())
                {
                    continue;
                }

                collideFlag->HandleCollectable(collidePlayer);
                ServerGame::Instance()->sendRemovePacket(ClassId::Flag,
                                                         collideFlag->GetObjectId(),
                                                         ClassId::Player,
                                                         collidePlayer->GetObjectId());
                markedList.push_back(collideFlag);
                collideFlag->SetToMarked(world_tick);
                // TODO send a packet for the player to acquire the item for GUI
            }

            // Handles Jump Semaphore
            else // if (++y % 50000 == 0)
            {
                int numContacts = contactManifold->getNumContacts();
                for (int j = 0; j < numContacts; j++)
                {
                    btManifoldPoint& pt = contactManifold->getContactPoint(j);
                    if (pt.getDistance() < 0.f)
                    {
                        const btVector3& ptA = pt.getPositionWorldOnA();
                        const btVector3& ptB = pt.getPositionWorldOnB();
                        const btVector3& normalOnB = pt.m_normalWorldOnB;

                        // Reset Jump Semaphore, detects collision off of non-player, error of .1
                        // TODO: FIX MAGIC NUMBER OF ClassId::Player HALFEXTENT = 1
                        if ((collidePlayer->GetEntityPosition()).getY() - 1 > (ptB.getY() - .1)
                            && (collidePlayer->GetEntityPosition()).getY() - 1 < (ptB.getY() + .1))
                        {
                            collidePlayer->ResetJump();
                        }

                        /*
                        Notes: No Rotation: Can minus halfextent of player object from pt of
                        collision ~= 0
                        Notes: Rotation:  Collision pt - (halfextent * normalOnB) ~= 0
                        printf(" ptA at (%f,%f,%f)\n", ptA.getX(), ptA.getY(), ptA.getZ());
                        printf(" ptB at (%f,%f,%f)\n", ptB.getX(), ptB.getY(), ptB.getZ());
                        printf(" normalOnB at (%f,%f,%f)\n", normalOnB.getX(), normalOnB.getY(),
                        normalOnB.getZ());
                        */
                    }
                }
            }
        }

        // Obj B is Player
        else if (static_cast<ClassId>(obB->getUserIndex()) == ClassId::Player)
        {
            // Grab Player Object
            Player* collidePlayer = (Player*)obB->getUserPointer();

            // If Obj A is collectable, HandleCollectable();
            if (static_cast<ClassId>(obA->getUserIndex()) == ClassId::Collectable)
            {
                // Ignore dead guy
                if (!collidePlayer->IsAlive())
                    continue;

                // Handle Collectable Collection
                Collectable* collectObj = (Collectable*)obA->getUserPointer();
                // check if collectable has been handled already
                if (collectObj->GetMarked())
                {
                    continue;
                }
                collectObj->HandleCollect(collidePlayer);
                if (collectObj->GetType() == CollectType::Weapon)
                {
                    ServerGame::Instance()->sendRemovePacket(
                        ClassId::Collectable,
                        collectObj->GetObjectId(),
                        ClassId::Player,
                        collidePlayer->GetObjectId(),
                        CollectType::Weapon,
                        static_cast<int>(collectObj->GetWeapon()->GetWeaponType()));
                }
                else if (collectObj->GetType() == CollectType::PowerUp)
                {
                    ServerGame::Instance()->sendRemovePacket(
                        ClassId::Collectable,
                        collectObj->GetObjectId(),
                        ClassId::Player,
                        collidePlayer->GetObjectId(),
                        CollectType::PowerUp,
                        static_cast<int>(collectObj->GetPowerUp()->getType()));
                }
                deleteList.push_back(collectObj);
                collectObj->SetToMarked(world_tick);
            }

            // if Obj A is Flag
            else if (static_cast<ClassId>(obA->getUserIndex()) == ClassId::Flag)
            {
                // Ignore dead guy
                if (!collidePlayer->IsAlive())
                    continue;

                // Handle Flag Collection
                Flag* collideFlag = (Flag*)obA->getUserPointer();
                if (collideFlag->GetMarked())
                {
                    continue;
                }
                collideFlag->HandleCollectable(collidePlayer);
                ServerGame::Instance()->sendRemovePacket(ClassId::Flag,
                                                         collideFlag->GetObjectId(),
                                                         ClassId::Player,
                                                         collidePlayer->GetObjectId());
                markedList.push_back(collideFlag);
                collideFlag->SetToMarked(world_tick);
                // TODO send a packet for the player to acquire the item for GUI
            }

            // Handles Jump Semaphore
            else // if (++y % 50000 == 0)
            {
                int numContacts = contactManifold->getNumContacts();
                for (int j = 0; j < numContacts; j++)
                {
                    btManifoldPoint& pt = contactManifold->getContactPoint(j);
                    if (pt.getDistance() < 0.f)
                    {
                        const btVector3& ptA = pt.getPositionWorldOnA();
                        const btVector3& ptB = pt.getPositionWorldOnB();
                        const btVector3& normalOnB = pt.m_normalWorldOnB;

                        // Reset Jump Semaphore, detects collision off of non-player, error of .1
                        // TODO: FIX MAGIC NUMBER OF ClassId::Player HALFEXTENT = 1
                        if ((collidePlayer->GetEntityPosition()).getY() - 1 > (ptA.getY() - .1)
                            && (collidePlayer->GetEntityPosition()).getY() - 1 < (ptA.getY() + .1))
                        {
                            collidePlayer->ResetJump();
                        }

                        /*
                        Notes: No Rotation: Can minus halfextent of player object from pt of
                        collision ~= 0
                        Notes: Rotation:  Collision pt - (halfextent * normalOnB) ~= 0
                        printf(" ptA at (%f,%f,%f)\n", ptA.getX(), ptA.getY(), ptA.getZ());
                        printf(" ptB at (%f,%f,%f)\n", ptB.getX(), ptB.getY(), ptB.getZ());
                        printf(" normalOnB at (%f,%f,%f)\n", normalOnB.getX(), normalOnB.getY(),
                        normalOnB.getZ());
                        */
                    }
                }
            }
        }
    }

    // Reset Marked Entities
    for (auto it = markedList.begin(); it != markedList.end(); it++)
    {
        (*it)->ResetMark();
        this->curWorld->removeCollisionObject((*it)->GetRigidBody());
    }
    markedList.clear();

    // Delete Marked Entities
    for (auto it = deleteList.begin(); it != deleteList.end(); it++)
    {
        delete (*it);
    }
    deleteList.clear();

    // Unmarks entities not on the previous lists, used for bullet collision
    for (auto it = unmarkList.begin(); it != unmarkList.end(); it++)
    {
        if ((*it)->GetMarkTick() == world_tick - 5)
            (*it)->ResetMark();
    }

    // if (x++ % 10000 == 0) {
    if (world_tick % 500 == 0)
    {
        /*
        btVector3 vecg = rightWall->getCenterOfMassPosition();
        printf(" right wall at (%f,%f,%f)\n", vecg.getX(), vecg.getY(), vecg.getZ());
        vecg = leftWall->getCenterOfMassPosition();
        printf(" left wall at (%f,%f,%f)\n", vecg.getX(), vecg.getY(), vecg.getZ());
        vecg = frontWall->getCenterOfMassPosition();
        printf(" front wall at (%f,%f,%f)\n", vecg.getX(), vecg.getY(), vecg.getZ());
        vecg = backWall->getCenterOfMassPosition();
        printf(" back wall at (%f,%f,%f)\n", vecg.getX(), vecg.getY(), vecg.getZ());
        */

        auto& dynamicMap = EntitySpawner::Instance()->GetMap();
        for (auto it = dynamicMap.begin(); it != dynamicMap.end(); it++)
        {
            btVector3 vec = it->second->GetEntityPosition();
            // printf(" Dynamic object classid: %d, objid: %d, at (%f,%f,%f)\n",
            // it->second->GetClassId(), it->second->GetObjectId(), vec.getX(), vec.getY(),
            // vec.getZ());
        }

        /*
        for (std::vector<std::shared_ptr<Flag> >::iterator it = flags.begin(); it != flags.end();
        ++it)
        {
            btVector3 vec = (*it)->GetEntityPosition();
            printf(" flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
        }
        */
    }

    // Handle spawning for this tick
    RespawnHandler::Instance()->RespawnPlayers(world_tick);
    CollectableSpawner::Instance()->SpawnRandomCollectables(curWorld, world_tick);

    // Send position updates of all dynamic objects
    if (world_tick % 4 == 0)
    {
        // Iterates through all dynamic objects in the Map and sends position updates to client
        auto& dynamicMap = EntitySpawner::Instance()->GetMap();
        for (auto it = dynamicMap.begin(); it != dynamicMap.end(); it++)
        {
            // btVector3 vec = it->second->GetEntityPosition();
            btVector3 vec = it->second->GetRigidBody()->getLinearVelocity();
            float thresh = .0005;
            // don't send packets if the object is stationary?
            if ((abs(vec.getX()) < thresh && abs(vec.getY()) < thresh && abs(vec.getZ()) < thresh))
            {
                if (static_cast<ClassId>(it->second->GetClassId()) == ClassId::Player)
                {
                    ServerGame::Instance()->sendMovePacket((ClassId)it->second->GetClassId(),
                                                           it->second->GetObjectId());
                }
                // printf(" Dynamic object classid: %d, objid: %d, velocity (%f,%f,%f)\n",
                // it->second->GetClassId(), it->second->GetObjectId(), vec.getX(), vec.getY(),
                // vec.getZ());
                continue;
            }

            // modify on player ticks
            if (static_cast<ClassId>(it->second->GetClassId()) == ClassId::Player)
            {
                // resets stuns
                if (((Player*)it->second)->GetStun() > 0)
                    ((Player*)it->second)
                        ->SetStun(((Player*)it->second)->GetStun() - 4); // unstun the guy

                // resets powerups
                if (((Player*)it->second)->GetPowerUpDuration() > 0)
                {
                    ((Player*)it->second)
                        ->SetPowerUpDuration(((Player*)it->second)->GetPowerUpDuration() - 4);
                    if (((Player*)it->second)->GetPowerUpDuration() <= 0)
                    {
                        if (((Player*)it->second)->GetPower() != nullptr)
                            ((Player*)it->second)->GetPower()->removePower(((Player*)it->second));
                    }
                }
            }

            ServerGame::Instance()->sendMovePacket((ClassId)it->second->GetClassId(),
                                                   it->second->GetObjectId());
        }
    }

    world_tick++;
}

void World::removeFlag(Flag* collectedFlag)
{
    for (auto it = flags.begin(); it != flags.end(); ++it)
    {
        if (collectedFlag == it->get())
        {
            flags.erase(it);
            printf("Flag has been removed from world list \n");
            return;
        }
    }
}
