#pragma once

#include "Player.h"
#include "Flag.h"
#include "WorldObstacle.h"
#include <vector>
#include <memory>

#include "../../network/GameData.h"
// Game Obstacle is already existing

using namespace std;

typedef vector<PosInfo> pos_list;

class World {

private:
	// list of map objects
	// list (vector) of players sorted by client id
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Flag>> flags;

	btDiscreteDynamicsWorld* curWorld;
	btDefaultCollisionConfiguration* colConfig;
	btCollisionDispatcher* disp;
	btBroadphaseInterface* pairCache;
	btSequentialImpulseConstraintSolver* solv;
	std::vector <btRigidBody*> bullets;
	//std::vector <btRigidBody*> players;
	WorldObstacle * ground;
	WorldObstacle * frontWall;
	WorldObstacle * backWall;
	WorldObstacle * leftWall;
	WorldObstacle * rightWall;

	// object ids
	int oid;

public:
	World();
	~World();

	// ticker used for now
	int x = 0;
	int y = 0;

	void Init();

	// Spawns based on PosInfo in, will return PosInfo for the object spawned
	PosInfo SpawnPlayer(PosInfo in);
	PosInfo SpawnFlag(PosInfo in);
	std::shared_ptr<Player> GetPlayer(int id) { return players.at(id); };

	// Updates Physics world by one tick
	// Handles egg collisions with players
	void updateWorld();

	// Finds and Removes flag from world list of flags
	void removeFlag(std::shared_ptr<Flag> collectedFlag);

};