#include "World.h"
#include "ObjectId.h"
#include "Bullet.h"
#include "../ServerGame.h"

World::World() {
	// initialize map objects 
}

World::~World() {
	//TODO handle vector deletes...
	delete curWorld;
}

void World::Init() {

	// Init object id counter
	oid = 0;
	currentWorldTick = 0;

	// Init Fire Rate Reseter
	this->fireRateReseter = new FireRateReset((&this->usedWeapons));

	int z = 1000; // this is a random number for the walls right now, we need to change this

	// Create Physics world
	btDefaultCollisionConfiguration * collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0, -0.1, 0));

	// Add Ground Object
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(1.), btScalar(0.)), 0);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_friction = .4;
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	groundRigidBody->setGravity(btVector3(0, -0.5, 0));
	dynamicsWorld->addRigidBody(groundRigidBody);
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
	// Create Neg Z Wall
	WorldObstacle* nzwall = new WorldObstacle(z++, nzWallRigidBody, curWorld);

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

	// Initialize player objects
	/*for (int i = 0; i < player_poss.size(); i++) {
		int teamid = 1;
		std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(oid, teamid, player_poss.at(i), curWorld));
		btVector3 vec = player->GetPlayerPosition();
		printf("Created player at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		//printf("Posinfo player at (%d,%d,%d)\n", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
		players.push_back(player);

		// Send spawn info to the clients
		PosInfo pi;
		pi.cid = ClassId::PLAYER;
		pi.oid = oid++;
		pi.x = vec.getX();
		pi.y = vec.getY();
		pi.z = vec.getZ();
		ServerGame::instance()->sendSpawnPacket(pi);
	}

	// Initialize egg objects
	for (int i = 0; i < flag_poss.size(); i++) {
		std::shared_ptr<Flag> flag = std::shared_ptr<Flag>(new Flag(oid, flag_poss.at(i), curWorld));
		btVector3 vec = flag->GetFlagPosition();
		printf("Created flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		printf("Posinfo flag at (%d,%d,%d)\n", flag->p.x, flag->p.y, flag->p.z);
		flags.push_back(flag);

		PosInfo pi;
		pi.cid = ClassId::FLAG;
		pi.oid = oid++;
		pi.x = vec.getX();
		pi.y = vec.getY();
		pi.z = vec.getZ();
		ServerGame::instance()->sendSpawnPacket(pi);
	}*/
}

PosInfo World::SpawnPlayer(PosInfo in)
{

	int teamid = 1;
	std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(oid, teamid, in, curWorld));
	btVector3 vec = player->GetPlayerPosition();
	printf("Created player at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
	//printf("Posinfo player at (%d,%d,%d)\n", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	players.push_back(player);

	btQuaternion quat = player->GetPlayerRotation();

	// Send spawn info to the clients
	PosInfo out;
	out.cid = ClassId::PLAYER;
	out.oid = oid++;
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	out.rotw = quat.getW();
	out.rotx = quat.getX();
	out.roty = quat.getY();
	out.rotz = quat.getZ();
	ServerGame::instance()->sendSpawnPacket(out);
	return out;
}

PosInfo World::SpawnFlag(PosInfo in)
{
	std::shared_ptr<Flag> flag = std::shared_ptr<Flag>(new Flag(oid, in, curWorld));
	btVector3 vec = flag->GetFlagPosition();
	printf("Created flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
	printf("Posinfo flag at (%d,%d,%d)\n", flag->p.x, flag->p.y, flag->p.z);
	flags.push_back(flag);

	PosInfo out;
	out.cid = ClassId::FLAG;
	out.oid = oid++;
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	ServerGame::instance()->sendSpawnPacket(out);
	return out;
}

		/*case BULLET:
		{
			std::shared_ptr<Bullet> bullet = std::shared_ptr<Bullet>(new Bullet(oid2, playeridforBullet, teamid, damageforBullet, position, speed, curWorld));
			btVector3 vec = bullet->GetBulletPosition();
			printf("Created flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
			bullets.push_back(bullet);

			PosInfo pi;
			pi.cid = ClassId::BULLET;
			pi.oid = oid2++;
			pi.x = vec.getX();
			pi.y = vec.getY();
			pi.z = vec.getZ();
			ServerGame::instance()->sendSpawnPacket(pi);
			break;
		}*/

void World::UpdateWorld()
{
	// Step simulation
	curWorld->stepSimulation(1 / 60.f, 10);
	currentWorldTick++;

	// Process Weapon Reloads
	this->fireRateReseter->ResetWeapons();

	// Process all collisions
	int numManifolds = curWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = curWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		// Handle Bullet Collisions--------------------------
		// If object A of collision is a Bullet
		if (obA->getUserIndex() == BULLET)
		{
			// Grab Bullet Object
			Bullet * collideBullet = (Bullet *)obA->getUserPointer();

			// If it hit a player
			if (obB->getUserIndex() == PLAYER)
			{
				Player * collidePlayer = (Player *)obB->getUserPointer();

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
				delete collideBullet2;
			}

			// deletes bulletA regardless
			delete collideBullet;
		}

		// If object B of collision is a Bullet
		else if (obB->getUserIndex() == BULLET)
		{
			// Grab Bullet Object
			Bullet * collideBullet = (Bullet *)obB->getUserPointer();

			// If it hit a player
			if (obA->getUserIndex() == PLAYER)
			{
				Player * collidePlayer = (Player *)obA->getUserPointer();
				if (collidePlayer->takeDamage(collideBullet->GetDamage()))
				{
					//TODO Handle Player death:  send player death to client
				}
			}

			// If it hit a bullet
			else if (obA->getUserIndex() == BULLET)
			{
				Bullet * collideBullet2 = (Bullet *)obA->getUserPointer();
				delete collideBullet2;
			}

			// deletes bulletA regardless
			delete collideBullet;
		}


		// Handle Player Collisions -------------------------------------------------
		// Obj A is Player
		if (obA->getUserIndex() == PLAYER)
		{
			// Grab Player Object
			Player * collidePlayer = (Player *)obA->getUserPointer();

			// if Obj B is Flag
			if (obB->getUserIndex() == FLAG)
			{
				// Handle Flag Collection
				Flag * collideFlag = (Flag *)obB->getUserPointer();

				collidePlayer->AcquireFlag((std::shared_ptr<Flag>)collideFlag);
				curWorld->removeRigidBody(collideFlag->getRigidBody());
				ServerGame::instance()->sendRemovePacket(ClassId::FLAG, collideFlag->GetObjectId());
				//TODO send a packet for the player to acquire the item

				//TODO remove flag from Vector causes strange issues...
				removeFlag(collideFlag);
			}
			//else if   TODO Handle Bullet Collision
			//...

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
						if ((collidePlayer->GetPlayerPosition()).getY() - 1 > (ptB.getY() - .1) &&
							(collidePlayer->GetPlayerPosition()).getY() - 1 < (ptB.getY() + .1))
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

			// if Obj A is Flag
			if ((obA->getUserIndex()) == FLAG)
			{
				// Handle Flag Collection
				Flag * collideFlag = (Flag *)obA->getUserPointer();

				collidePlayer->AcquireFlag((std::shared_ptr<Flag>)collideFlag);
				curWorld->removeRigidBody(collideFlag->getRigidBody());
				ServerGame::instance()->sendRemovePacket(ClassId::FLAG, collideFlag->GetObjectId());
				//TODO send a packet for the player to acquire the item

				//TODO remove flag from Vector causes strange issues...
				removeFlag(collideFlag);
			}
			//else if   TODO Handle Bullet Collision
			//...

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
						if ((collidePlayer->GetPlayerPosition()).getY() - 1 > (ptA.getY()- .1) &&
							(collidePlayer->GetPlayerPosition()).getY() - 1 < (ptA.getY() + .1))
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
	if (x++ % 10000 == 0) {

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

		for (std::vector<std::shared_ptr<Player> >::iterator it = players.begin(); it != players.end(); ++it)
		{
			btVector3 vec = (*it)->GetPlayerPosition();
			printf(" player at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		}

		/*
		for (std::vector<std::shared_ptr<Flag> >::iterator it = flags.begin(); it != flags.end(); ++it)
		{
			btVector3 vec = (*it)->GetFlagPosition();
			printf(" flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		}
		*/
	}

	// send updates every x or so ticks?
	if (x % 5 == 0)
	{
		for (std::shared_ptr<Player>& player : players)
		{
			ServerGame::instance()->sendMovePacket(ClassId::PLAYER, player->GetId());
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
