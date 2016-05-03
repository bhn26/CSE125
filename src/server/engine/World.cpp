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
	
	/*	
	// Add Pos X Wall
	btCollisionShape* xWallShape = new btStaticPlaneShape(btVector3(btScalar(1.), btScalar(0.), btScalar(0.)), -100);
	btDefaultMotionState* xWallMotionState = new btDefaultMotionState();// btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo xWallRigidBodyCI(0, xWallMotionState, xWallShape, btVector3(0, 0, 0));
	xWallRigidBodyCI.m_friction = .5;
	btRigidBody* xWallRigidBody = new btRigidBody(xWallRigidBodyCI);
	dynamicsWorld->addRigidBody(xWallRigidBody);

	// Add Neg X Wall
	btCollisionShape* nxWallShape = new btStaticPlaneShape(btVector3(btScalar(-WORLD_WIDTH), btScalar(0.), btScalar(0.)), -WORLD_WIDTH);
	btDefaultMotionState* nxWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo nxWallRigidBodyCI(0, nxWallMotionState, nxWallShape, btVector3(0, 0, 0));
	nxWallRigidBodyCI.m_friction = .5;
	btRigidBody* nxWallRigidBody = new btRigidBody(nxWallRigidBodyCI);
	dynamicsWorld->addRigidBody(nxWallRigidBody);

	// Add Pos Z Wall
	btCollisionShape* zWallShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(0.), btScalar(WORLD_WIDTH)), WORLD_WIDTH);
	btDefaultMotionState* zWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo zWallRigidBodyCI(0, zWallMotionState, zWallShape, btVector3(0, 0, 0));
	zWallRigidBodyCI.m_friction = .5;
	btRigidBody* zWallRigidBody = new btRigidBody(zWallRigidBodyCI);
	dynamicsWorld->addRigidBody(zWallRigidBody);

	// Add Neg Z Wall
	btCollisionShape* nzWallShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(0.), btScalar(-WORLD_WIDTH)), -WORLD_WIDTH);
	btDefaultMotionState* nzWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo nzWallRigidBodyCI(0, nzWallMotionState, nzWallShape, btVector3(0, 0, 0));
	nzWallRigidBodyCI.m_friction = .5;
	btRigidBody* nzWallRigidBody = new btRigidBody(nzWallRigidBodyCI);
	dynamicsWorld->addRigidBody(nzWallRigidBody);
	*/

	// Set Local attributes
	ground = groundRigidBody;
	/*
	frontWall = nzWallRigidBody;
	backWall = zWallRigidBody;
	leftWall = nxWallRigidBody;
	rightWall = xWallRigidBody;
	*/
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
	curWorld->stepSimulation(1 / 60.f, 10);

	int numManifolds = curWorld->getDispatcher()->getNumManifolds();

	/*
	if (indexCounter == 5000) 
	{
		btVector3 playerPos = (players.at(0))->GetPlayerPosition();
		printf("world pos object = %f,%f,%f\n", float(playerPos.getX()), float(playerPos.getY()), float(playerPos.getZ()));
		(players.at(0))->PrintPlayerVelocity();
		indexCounter = 0;
	}
	indexCounter++;
	*/
	
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = curWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		// Obj A is Flag
		if ((obA->getUserIndex()) == FLAG)
		{
			// Obj B is Player
			if (obB->getUserIndex() == PLAYER)
			{

				// Handle Flag Collection

				Flag * collideFlag = (Flag *)obB->getUserPointer();
				Player * collidePlayer = (Player *)obA->getUserPointer();

				collidePlayer->AcquireFlag((std::shared_ptr<Flag>)collideFlag);
				curWorld->removeRigidBody(collideFlag->getRigidBody());
				//TODO remove flag from Vector causes strange issues...
				//removeFlag((std::shared_ptr<Flag>)collideFlag);
			}
		}
		// Obj B is Flag
		else if (obB->getUserIndex() == FLAG)
		{
			// Obj A is Player
			if (obA->getUserIndex() == PLAYER)
			{

				// Handle Flag Collection
				//std::shared_ptr<Flag>* collideFlag = reinterpret_cast<std::shared_ptr<Flag> * >(obB->getUserPointer());
				//std::shared_ptr<Player>* collidePlayer = reinterpret_cast<std::shared_ptr<Player> * >(obA->getUserPointer());

				//(*collidePlayer)->AcquireFlag(*collideFlag);

				Flag * collideFlag = (Flag *)obB->getUserPointer();
				Player * collidePlayer = (Player *)obA->getUserPointer();

				collidePlayer->AcquireFlag((std::shared_ptr<Flag>)collideFlag);
				curWorld->removeRigidBody(collideFlag->getRigidBody());
				//TODO remove flag from Vector causes strange issues...
				//removeFlag((std::shared_ptr<Flag>)collideFlag);
			}
		}
	}
	if (x++ % 10000 == 0) {

		for (std::vector<std::shared_ptr<Player> >::iterator it = players.begin(); it != players.end(); ++it)
		{
			btVector3 vec = (*it)->GetPlayerPosition();
			printf(" player at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		}

		for (std::vector<std::shared_ptr<Flag> >::iterator it = flags.begin(); it != flags.end(); ++it)
		{
			btVector3 vec = (*it)->GetFlagPosition();
			printf(" flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
		}
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
