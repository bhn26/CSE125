
#include "World.h"
#include "ObjectId.h"
#include "../ServerGame.h"
#include "FireRateReset.h"
#include "Player.h"
#include "Flag.h"
#include "Bullet.h"
#include "WorldObstacle.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

World::World() {
	// initialize map objects 
}

World::~World() {
	//TODO handle vector deletes...
	delete curWorld;
}

void World::Init() {

	// Init world tick counter
	currentWorldTick = 0;

	// Init Fire Rate Reseter
	FireRateReset::instance();

	int z = 1000; // this is a random number for the walls right now, we need to change this

	// Create Physics world
	btDefaultCollisionConfiguration * collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0, -.1, 0));

	// Add Ground Object
	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(1.), btScalar(0.)), 0);
	btCollisionShape* groundShape = new btBoxShape(btVector3(WORLD_WIDTH, 1, WORLD_WIDTH));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_friction = .4;
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);
	groundRigidBody->setGravity(btVector3(0, .1, 0));
	groundRigidBody->setUserIndex(14);
	// Create Ground Obstacle
	WorldObstacle* groundwall = new WorldObstacle(z++, groundRigidBody, curWorld);
	
	// Add Pos X Wall
	btCollisionShape* xWallShape = new btStaticPlaneShape(btVector3(btScalar(-1.), btScalar(0.), btScalar(0.)), 0);
	btDefaultMotionState* xWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(WORLD_WIDTH, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo xWallRigidBodyCI(0, xWallMotionState, xWallShape, btVector3(0, 0, 0));
	xWallRigidBodyCI.m_friction = .5;
	btRigidBody* xWallRigidBody = new btRigidBody(xWallRigidBodyCI);
	xWallRigidBody->setGravity(btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(xWallRigidBody);
	xWallRigidBody->setUserIndex(5);
	// Create X Wall
	WorldObstacle* xwall = new WorldObstacle(z++, xWallRigidBody, curWorld);

	// Add Neg X Wall
	btCollisionShape* nxWallShape = new btStaticPlaneShape(btVector3(btScalar(1.), btScalar(0.), btScalar(0.)), 0);
	btDefaultMotionState* nxWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-WORLD_WIDTH, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo nxWallRigidBodyCI(0, nxWallMotionState, nxWallShape, btVector3(0, 0, 0));
	nxWallRigidBodyCI.m_friction = .5;
	btRigidBody* nxWallRigidBody = new btRigidBody(nxWallRigidBodyCI);
	nxWallRigidBody->setGravity(btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(nxWallRigidBody);
	nxWallRigidBody->setUserIndex(6);
	// Create Neg X Wall
	WorldObstacle* nxwall = new WorldObstacle(z++, groundRigidBody, curWorld);

	// Add Pos Z Wall
	btCollisionShape* zWallShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(0.), btScalar(-1.)), 0);
	btDefaultMotionState* zWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, WORLD_WIDTH)));
	btRigidBody::btRigidBodyConstructionInfo zWallRigidBodyCI(0, zWallMotionState, zWallShape, btVector3(0, 0, 0));
	zWallRigidBodyCI.m_friction = .5;
	btRigidBody* zWallRigidBody = new btRigidBody(zWallRigidBodyCI);
	zWallRigidBody->setGravity(btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(zWallRigidBody);
	zWallRigidBody->setUserIndex(7);
	// Create Pos Z Wall
	WorldObstacle* zwall = new WorldObstacle(z++, zWallRigidBody, curWorld);

	// Add Neg Z Wall
	btCollisionShape* nzWallShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(0.), btScalar(1.)), 0);
	btDefaultMotionState* nzWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -WORLD_WIDTH)));
	btRigidBody::btRigidBodyConstructionInfo nzWallRigidBodyCI(0, nzWallMotionState, nzWallShape, btVector3(0, 0, 0));
	nzWallRigidBodyCI.m_friction = .5;
	btRigidBody* nzWallRigidBody = new btRigidBody(nzWallRigidBodyCI);
	nzWallRigidBody->setGravity(btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(nzWallRigidBody);
	nzWallRigidBody->setUserIndex(8);
	// Create Neg Z Wall
	WorldObstacle* nzwall = new WorldObstacle(z++, nzWallRigidBody, curWorld);



	btCollisionShape* playerShape = new btBoxShape(btVector3(6, 2, 5));
	// Create tractor physics object
	btDefaultMotionState*playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(16.5, 1, -13)));
	btScalar mass = 100;
	btVector3 playerInertia(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(mass, playerMotionState, playerShape, playerInertia);
	btRigidBody* pRigidBody = new btRigidBody(playerRigidBodyCI);
	pRigidBody->setFriction((btScalar)0.5);
	pRigidBody->setDamping((btScalar)100, (btScalar)100);
	dynamicsWorld->addRigidBody(pRigidBody);
	pRigidBody->setUserIndex(9);

	playerShape = new btBoxShape(btVector3(3, 5, 3));
	// Create tractor physics object
	playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-13, 1, -2.5)));
	mass = 100;
	playerInertia = btVector3(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo p(mass, playerMotionState, playerShape, playerInertia);
	pRigidBody = new btRigidBody(p);
	pRigidBody->setFriction((btScalar)0.5);
	pRigidBody->setDamping((btScalar)100, (btScalar)100);
	dynamicsWorld->addRigidBody(pRigidBody);
	pRigidBody->setUserIndex(10);

	playerShape = new btBoxShape(btVector3(2, 8, 10));
	// Create tractor physics object
	playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(7.5, 1, 9)));
	mass = 100;
	playerInertia = btVector3(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo p1(mass, playerMotionState, playerShape, playerInertia);
	pRigidBody = new btRigidBody(p1);
	pRigidBody->setFriction((btScalar)0.5);
	pRigidBody->setDamping((btScalar)100, (btScalar)100);
	dynamicsWorld->addRigidBody(pRigidBody);
	pRigidBody->setUserIndex(11);

	playerShape = new btBoxShape(btVector3(10, 8, 2));
	// Create tractor physics object
	playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(.1, 1, 17.5)));
	mass = 100;
	playerInertia = btVector3(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo p2(mass, playerMotionState, playerShape, playerInertia);
	pRigidBody = new btRigidBody(p2);
	pRigidBody->setFriction((btScalar)0.5);
	pRigidBody->setDamping((btScalar)100, (btScalar)100);
	dynamicsWorld->addRigidBody(pRigidBody);
	pRigidBody->setUserIndex(12);

	playerShape = new btBoxShape(btVector3(2, 8, 10));
	// Create tractor physics object
	playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-8, 1, 9.7)));
	mass = 100;
	playerInertia = btVector3(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo p3(mass, playerMotionState, playerShape, playerInertia);
	pRigidBody = new btRigidBody(p3);
	pRigidBody->setFriction((btScalar)0.5);
	pRigidBody->setDamping((btScalar)100, (btScalar)100);
	dynamicsWorld->addRigidBody(pRigidBody);
	pRigidBody->setUserIndex(13);

	playerShape = new btBoxShape(btVector3(11, 2, 11));
	// Create tractor physics object
	playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(.5, 10, 8)));
	mass = 100;
	playerInertia = btVector3(0, 0, 0);
	playerShape->calculateLocalInertia(mass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo p4(mass, playerMotionState, playerShape, playerInertia);
	pRigidBody = new btRigidBody(p4);
	pRigidBody->setFriction((btScalar)0.5);
	pRigidBody->setDamping((btScalar)100, (btScalar)100);
	dynamicsWorld->addRigidBody(pRigidBody);
	pRigidBody->setUserIndex(14);

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
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

}

btDiscreteDynamicsWorld* World::GetPhysicsWorld()
{
	return curWorld;
}

void World::PreSpawn()
{
	ServerGame::instance()->sendReadyToSpawnPacket();
}

void World::UpdateWorld()
{
	// Step simulation
	curWorld->stepSimulation(1 / 60.f, 10);
	currentWorldTick++;

	// Process Weapon Fire Rate Reset
	FireRateReset::instance()->ResetWeapons();

	// Process all collisions
	int numManifolds = curWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		//printf("numMan  %d\n", numManifolds);
		btPersistentManifold* contactManifold = curWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		// Handle Bullet Collisions--------------------------
		// If object A of collision is a Bullet
		if (obA->getUserIndex() == BULLET)
		{
			// Grab Bullet Object
			Bullet * collideBullet = (Bullet *)obA->getUserPointer();
			if (collideBullet->MarkStatus())
			{
				continue;
			}

			// If it hit a player
			if (obB->getUserIndex() == PLAYER)
			{
				Player * collidePlayer = (Player *)obB->getUserPointer();

				printf("Pushed to delete!, hit playerB");
				deleteList.push_back(collideBullet);
				collideBullet->SetToMarked();
				//TODO send "you got hit"
				if (collidePlayer->takeDamage(collideBullet->GetDamage()))
				{
					printf("Player is dead!");
					//TODO Handle Player death:  send player death to client
				}
			}

			// If it hit a bullet
			else if (obB->getUserIndex() == BULLET)
			{
				Bullet * collideBullet2 = (Bullet *)obB->getUserPointer();
				printf("Pushed to delete!, hit bullet B");
				deleteList.push_back(collideBullet);
				collideBullet->SetToMarked();
				//delete collideBullet2;
			}
			else if (obB->getUserIndex() < 15)
			{
				// deletes bulletA regardless
				printf("Pushed to delete!, hit ground B,  %d", obB->getUserIndex());
				btVector3 bulPos = collideBullet->GetEntityPosition();
				printf("Current position:  x: %f, y: %f, z: %f  \n", bulPos.getX(), bulPos.getY(), bulPos.getZ());
				bulPos = collideBullet->GetRigidBody()->getLinearVelocity();
				printf("Current velocity:  x: %f, y: %f, z: %f  \n", bulPos.getX(), bulPos.getY(), bulPos.getZ());

				deleteList.push_back(collideBullet);
				collideBullet->SetToMarked();
			}
		}

		// If object B of collision is a Bullet
		else if (obB->getUserIndex() == BULLET)
		{
			// Grab Bullet Object
			Bullet * collideBullet = (Bullet *)obB->getUserPointer();
			if (collideBullet->MarkStatus())
			{
				continue;
			}

			// Bullet hits player
			if (obA->getUserIndex() == PLAYER)
			{
				Player * collidePlayer = (Player *)obA->getUserPointer();

				printf("Pushed to delete! hit player A");
				deleteList.push_back(collideBullet);
				collideBullet->SetToMarked();
				if (collidePlayer->takeDamage(collideBullet->GetDamage()))
				{
					printf("Player is dead!");
					//TODO Handle Player death:  send player death to client
				}
			}

			// If it hit a bullet
			else if (obA->getUserIndex() == BULLET)
			{
				Bullet * collideBullet2 = (Bullet *)obA->getUserPointer();
				printf("Pushed to delete! hit bullet A");
				deleteList.push_back(collideBullet);
				collideBullet->SetToMarked();
			}
			else if (obA->getUserIndex() < 15)
			{
				// deletes bulletB regardless
				printf("Pushed to delete!, hit ground A,  %d", obA->getUserIndex());
				btVector3 bulPos = collideBullet->GetEntityPosition();
				printf("Current position:  x: %f, y: %f, z: %f  \n", bulPos.getX(), bulPos.getY(), bulPos.getZ());
				bulPos = collideBullet->GetRigidBody()->getLinearVelocity();
				printf("Current velocity:  x: %f, y: %f, z: %f  \n", bulPos.getX(), bulPos.getY(), bulPos.getZ());

				deleteList.push_back(collideBullet);
				collideBullet->SetToMarked();
			}
		}


		// Handle Player Collisions -------------------------------------------------
		// Obj A is Player
		if (obA->getUserIndex() == PLAYER)
		{
			// Grab Player Object
			Player * collidePlayer = (Player *)obA->getUserPointer();

			// TODO, if Obj B is collectable, HandleCollectable();
			if (obB->getUserIndex() == COLLECTABLE)
			{

				// Handle Collectable Collection
				Collectable* collectObj = (Collectable*)obB->getUserPointer();
				collectObj->HandleCollect(collidePlayer);
				ServerGame::instance()->sendRemovePacket(ClassId::COLLECTABLE, collectObj->GetObjectId());
			}

			// if Obj B is Flag
			else if (obB->getUserIndex() == FLAG)
			{

				// Handle Flag Collection
				Flag * collideFlag = (Flag *)obB->getUserPointer();
				if (collideFlag->MarkStatus())
				{
					continue;
				}
				collideFlag->HandleCollectable(collidePlayer);
				ServerGame::instance()->sendRemovePacket(ClassId::FLAG, collideFlag->GetObjectId());
				markedList.push_back(collideFlag);
				collideFlag->SetToMarked();
				//TODO send a packet for the player to acquire the item for GUI
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
						// TODO: FIX MAGIC NUMBER OF PLAYER HALFEXTENT = 1
						if ((collidePlayer->GetEntityPosition()).getY() - 1 > (ptB.getY() - .1) &&
							(collidePlayer->GetEntityPosition()).getY() - 1 < (ptB.getY() + .1))
						{
							collidePlayer->ResetJump();
						}
						
						/*
						Notes: No Rotation: Can minus halfextent of player object from pt of collision ~= 0
						Notes: Rotation:  Collision pt - (halfextent * normalOnB) ~= 0
						printf(" ptA at (%f,%f,%f)\n", ptA.getX(), ptA.getY(), ptA.getZ());
						printf(" ptB at (%f,%f,%f)\n", ptB.getX(), ptB.getY(), ptB.getZ());
						printf(" normalOnB at (%f,%f,%f)\n", normalOnB.getX(), normalOnB.getY(), normalOnB.getZ());
						*/
					}
				}
			}
		}

		// Obj B is Player
		else if (obB->getUserIndex() == PLAYER)
		{
			// Grab Player Object
			Player * collidePlayer = (Player *)obB->getUserPointer();

			// If Obj A is collectable, HandleCollectable();
			if (obA->getUserIndex() == COLLECTABLE)
			{
				// Handle Collectable Collection
				Collectable* collectObj = (Collectable*)obA->getUserPointer();
				collectObj->HandleCollect(collidePlayer);
				ServerGame::instance()->sendRemovePacket(ClassId::COLLECTABLE, collectObj->GetObjectId());
			}

			// if Obj A is Flag
			else if (obA->getUserIndex() == FLAG)
			{
				// Handle Flag Collection
				Flag * collideFlag = (Flag *)obA->getUserPointer();
				if (collideFlag->MarkStatus())
				{
					continue;
				}
				collideFlag->HandleCollectable(collidePlayer);
				ServerGame::instance()->sendRemovePacket(ClassId::FLAG, collideFlag->GetObjectId());
				markedList.push_back(collideFlag);
				collideFlag->SetToMarked();
				//TODO send a packet for the player to acquire the item for GUI
			}

			// Handles Jump Semaphore
			else //if (++y % 50000 == 0) 
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
						// TODO: FIX MAGIC NUMBER OF PLAYER HALFEXTENT = 1
						if ((collidePlayer->GetEntityPosition()).getY() - 1 > (ptA.getY()- .1) &&
							(collidePlayer->GetEntityPosition()).getY() - 1 < (ptA.getY() + .1))
						{
							collidePlayer->ResetJump();
						}

						/*
						Notes: No Rotation: Can minus halfextent of player object from pt of collision ~= 0
						Notes: Rotation:  Collision pt - (halfextent * normalOnB) ~= 0
						printf(" ptA at (%f,%f,%f)\n", ptA.getX(), ptA.getY(), ptA.getZ());
						printf(" ptB at (%f,%f,%f)\n", ptB.getX(), ptB.getY(), ptB.getZ());
						printf(" normalOnB at (%f,%f,%f)\n", normalOnB.getX(), normalOnB.getY(), normalOnB.getZ());
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


	//if (x++ % 10000 == 0) {
	if (x++ % 500 == 0) {
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

		// TODO!!! HACK TO PRINT OUT UPDATE FOR ONE PLAYER.  Change to use Map in EntitySpawner

		std::map<std::pair<int,unsigned int>, Entity* > * dynamicMap = EntitySpawner::instance()->GetMap();
		for (std::map<std::pair<int, unsigned int>, Entity*>::iterator it = dynamicMap->begin(); it != dynamicMap->end(); it++)
		{
			btVector3 vec = it->second->GetEntityPosition();
			printf(" Dynamic object classid: %d, objid: %d, at (%f,%f,%f)\n", it->second->GetClassId(), it->second->GetObjectId(), vec.getX(), vec.getY(), vec.getZ());
		}

		/*
		for (std::vector<std::shared_ptr<Flag> >::iterator it = flags.begin(); it != flags.end(); ++it)
		{
			btVector3 vec = (*it)->GetEntityPosition();
			printf(" flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		}
		*/
	}

	// Send position updates of all dynamic objects
	if (x % 5 == 0)
	{
		// Iterates through all dynamic objects in the Map and sends position updates to client
		std::map<std::pair<int, unsigned int>, Entity* > * dynamicMap = EntitySpawner::instance()->GetMap();
		for (std::map<std::pair<int, unsigned int>, Entity*>::iterator it = dynamicMap->begin(); it != dynamicMap->end(); it++)
		{
			btVector3 vec = it->second->GetEntityPosition();
			//printf(" Dynamic object classid: %d, objid: %d, at (%f,%f,%f)\n", it->second->GetClassId(), it->second->GetObjectId(), vec.getX(), vec.getY(), vec.getZ());
			ServerGame::instance()->sendMovePacket((ClassId)it->second->GetClassId(), it->second->GetObjectId());
		}
	}
	
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
