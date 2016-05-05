#include "World.h"
#include "ObjectId.h"

World::World() {
	// initialize map objects 
}

World::~World() {

}

void World::Init(pos_list player_poss, pos_list flag_poss) {

	// Create Physics world
	btDefaultCollisionConfiguration * collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// Add Ground Object
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(1.), btScalar(0.)), 0);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_friction = .5;
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);
	
	
	// Add Pos X Wall
	btCollisionShape* xWallShape = new btStaticPlaneShape(btVector3(btScalar(-1.), btScalar(0.), btScalar(0.)), 0);
	btDefaultMotionState* xWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(WORLD_WIDTH, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo xWallRigidBodyCI(0, xWallMotionState, xWallShape, btVector3(0, 0, 0));
	xWallRigidBodyCI.m_friction = .5;
	btRigidBody* xWallRigidBody = new btRigidBody(xWallRigidBodyCI);
	xWallRigidBody->setGravity(btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(xWallRigidBody);

	
	// Add Neg X Wall
	btCollisionShape* nxWallShape = new btStaticPlaneShape(btVector3(btScalar(1.), btScalar(0.), btScalar(0.)), 0);
	btDefaultMotionState* nxWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-WORLD_WIDTH, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo nxWallRigidBodyCI(0, nxWallMotionState, nxWallShape, btVector3(0, 0, 0));
	nxWallRigidBodyCI.m_friction = .5;
	btRigidBody* nxWallRigidBody = new btRigidBody(nxWallRigidBodyCI);
	xWallRigidBody->setGravity(btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(nxWallRigidBody);

	// Add Pos Z Wall
	btCollisionShape* zWallShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(0.), btScalar(-1.)), 0);
	btDefaultMotionState* zWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, WORLD_WIDTH)));
	btRigidBody::btRigidBodyConstructionInfo zWallRigidBodyCI(0, zWallMotionState, zWallShape, btVector3(0, 0, 0));
	zWallRigidBodyCI.m_friction = .5;
	btRigidBody* zWallRigidBody = new btRigidBody(zWallRigidBodyCI);
	xWallRigidBody->setGravity(btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(zWallRigidBody);

	// Add Neg Z Wall
	btCollisionShape* nzWallShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(0.), btScalar(1.)), 0);
	btDefaultMotionState* nzWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -WORLD_WIDTH)));
	btRigidBody::btRigidBodyConstructionInfo nzWallRigidBodyCI(0, nzWallMotionState, nzWallShape, btVector3(0, 0, 0));
	nzWallRigidBodyCI.m_friction = .5;
	btRigidBody* nzWallRigidBody = new btRigidBody(nzWallRigidBodyCI);
	xWallRigidBody->setGravity(btVector3(0, 0, 0));
	dynamicsWorld->addRigidBody(nzWallRigidBody);
	

	// Set Local attributes
	ground = groundRigidBody;
	frontWall = nzWallRigidBody;
	backWall = zWallRigidBody;
	leftWall = nxWallRigidBody;
	rightWall = xWallRigidBody;
	
	curWorld = dynamicsWorld;
	solv = solver;
	pairCache = overlappingPairCache;
	disp = dispatcher;
	colConfig = collisionConfig;

	// Initialize player objects
	for (int i = 0; i < player_poss.size(); i++) {
		std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(i, player_poss.at(i), curWorld));
		btVector3 vec = player->GetPlayerPosition();
		printf("Created player at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		printf("Posinfo player at (%d,%d,%d)\n", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
		players.push_back(player);
	}

	// Initialize egg objects
	for (int i = 0; i < flag_poss.size(); i++) {
		std::shared_ptr<Flag> flag = std::shared_ptr<Flag>(new Flag(i, flag_poss.at(i), curWorld));
		btVector3 vec = flag->GetFlagPosition();
		printf("Created flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		printf("Posinfo flag at (%d,%d,%d)\n", flag->p.x, flag->p.y, flag->p.z);
		flags.push_back(flag);
	}
}

void World::updateWorld()
{
	// Step simulation
	curWorld->stepSimulation(1 / 60.f, 10);

	// Process all collisions
	int numManifolds = curWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = curWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		//Notes: check if ObA is Player
		//			if ObB Flag
		//          else if obB is Bullet
		//          else if collision is from the bottom, reset jump semaphore


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
				//TODO remove flag from Vector causes strange issues...
				//removeFlag((std::shared_ptr<Flag>)collideFlag);
			}
			//else if   TODO Handle Bullet Collision
			//...

			// Handles Jump Semaphore
			else // if (++y % 50000 == 0) 
			{
				int numContacts = contactManifold->getNumContacts();
				printf("numContacts are %d \n", numContacts);
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
				//TODO remove flag from Vector causes strange issues...
				//removeFlag((std::shared_ptr<Flag>)collideFlag);
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
	
	
}

void World::removeFlag(std::shared_ptr<Flag> collectedFlag)
{

	for (std::vector<std::shared_ptr<Flag> >::iterator it = flags.begin(); it != flags.end(); ++it)
	{
		if (collectedFlag == (*it))
		{
			flags.erase(it);
			printf("Flag has been removed from world list \n");
			return;
		}
	}

}
