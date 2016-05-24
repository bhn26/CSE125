#pragma once

#include "EntitySpawner.h"
#include <vector>
#include <memory>
#include <map>

#include "../../network/GameData.h"
class WorldObstacle;

using namespace std;

typedef vector<PosInfo> pos_list;

class World {

private:

	// list of game world objects
	std::vector<std::shared_ptr<Player>> players;   
	std::vector<std::shared_ptr<Flag>> flags;
	//std::vector<std::shared_ptr<Weapon>> usedWeapons;
	std::vector<std::shared_ptr<Bullet>> bullets;  // this may not be necessary...
	//TODO list of weapons to check reload

	// Physics World attributes
	btDiscreteDynamicsWorld* curWorld;
	btDefaultCollisionConfiguration* colConfig;
	btCollisionDispatcher* disp;
	btBroadphaseInterface* pairCache;
	btSequentialImpulseConstraintSolver* solv;

	// Map objects
	WorldObstacle * ground;
	WorldObstacle * frontWall;
	WorldObstacle * backWall;
	WorldObstacle * leftWall;
	WorldObstacle * rightWall;

	// object ids
	int oid;

	// list of fields to check.  Explosions and mounts.  Maybe decouple into a class of it's own.  Have a TTL


public:
	World();
	~World();

	// ticker used for now
	int x = 0;
	int y = 0;

	void Init();

	//TODO: Replace references with EntitySpawner  GetEntity() method
	std::shared_ptr<Player> GetPlayer(int id) { return players.at(id); };

	btDiscreteDynamicsWorld* GetPhysicsWorld();

	// Updates Physics world by one tick
	void UpdateWorld();

	// Finds and Removes flag from world list of flags
	void removeFlag(Flag* collectedFlag);

};