#include "World.h"

World::World() {
	// initialize map objects 
}

World::~World() {

}

void World::Init(pos_list player_poss) {

	// Create Physics world
	btDefaultCollisionConfiguration * collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// Add Ground Object
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(btScalar(0.), btScalar(1.), btScalar(0.)), 1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_friction = .9;
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	// Add Pos X Wall
	btCollisionShape* xWallShape = new btStaticPlaneShape(btVector3(btScalar(10.), btScalar(0.), btScalar(0.)), 10);
	btDefaultMotionState* xWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo xWallRigidBodyCI(0, xWallMotionState, xWallShape, btVector3(0, 0, 0));
	xWallRigidBodyCI.m_friction = .9;
	btRigidBody* xWallRigidBody = new btRigidBody(xWallRigidBodyCI);
	dynamicsWorld->addRigidBody(xWallRigidBody);

	// Add Neg X Wall
	btCollisionShape* nxWallShape = new btStaticPlaneShape(btVector3(btScalar(10.), btScalar(0.), btScalar(0.)), 10);
	btDefaultMotionState* nxWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo nxWallRigidBodyCI(0, nxWallMotionState, nxWallShape, btVector3(0, 0, 0));
	nxWallRigidBodyCI.m_friction = .9;
	btRigidBody* nxWallRigidBody = new btRigidBody(nxWallRigidBodyCI);
	dynamicsWorld->addRigidBody(nxWallRigidBody);

	// Add Pos Z Wall
	btCollisionShape* zWallShape = new btStaticPlaneShape(btVector3(btScalar(10.), btScalar(0.), btScalar(0.)), 10);
	btDefaultMotionState* zWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo zWallRigidBodyCI(0, zWallMotionState, zWallShape, btVector3(0, 0, 0));
	zWallRigidBodyCI.m_friction = .9;
	btRigidBody* zWallRigidBody = new btRigidBody(zWallRigidBodyCI);
	dynamicsWorld->addRigidBody(zWallRigidBody);

	// Add Neg Z Wall
	btCollisionShape* nzWallShape = new btStaticPlaneShape(btVector3(btScalar(10.), btScalar(0.), btScalar(0.)), 10);
	btDefaultMotionState* nzWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo nzWallRigidBodyCI(0, nzWallMotionState, nzWallShape, btVector3(0, 0, 0));
	nzWallRigidBodyCI.m_friction = .9;
	btRigidBody* nzWallRigidBody = new btRigidBody(nzWallRigidBodyCI);
	dynamicsWorld->addRigidBody(nzWallRigidBody);

	// Set Local attributes
	ground = groundRigidBody;
	curWorld = dynamicsWorld;
	solv = solver;
	pairCache = overlappingPairCache;
	disp = dispatcher;
	colConfig = collisionConfig;

	// Initialize player objects
	for (int i = 0; i < player_poss.size(); i++) {
		std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(i, player_poss.at(i)));
		players.push_back(player);
	}
}