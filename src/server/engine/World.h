#pragma once

#include "Weapon.h"
#include "Player.h"
#include "Flag.h"
#include "Bullet.h"
#include "WorldObstacle.h"
#include "FireRateReset.h"
#include <vector>
#include <memory>

#include "../../network/GameData.h"
// Game Obstacle is already existing

using namespace std;

typedef vector<PosInfo> pos_list;

class World {

private:

	// list of game world objects
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Flag>> flags;
	std::vector<std::shared_ptr<Weapon>> usedWeapons;
	std::vector<std::shared_ptr<Bullet>> bullets;  // this may not be necessary...
	//TODO list of weapons to check reload

	// Fire Rate Reseter
	FireRateReset * fireRateReseter;

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

	// Spawns based on PosInfo in, will return PosInfo for the object spawned
	PosInfo SpawnPlayer(PosInfo in);
	PosInfo SpawnFlag(PosInfo in);
	std::shared_ptr<Player> GetPlayer(int id) { return players.at(id); };

	// Updates Physics world by one tick
	// Handles egg collisions with players
	void UpdateWorld();

	// Finds and Removes flag from world list of flags
	void removeFlag(Flag* collectedFlag);

};