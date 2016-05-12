#pragma once

#include "Player.h"
#include "Flag.h"
#include "Bullet.h"
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
	std::vector<std::shared_ptr<Bullet>> bullets;  // this may not be necessary...

	btDiscreteDynamicsWorld* curWorld;
	btDefaultCollisionConfiguration* colConfig;
	btCollisionDispatcher* disp;
	btBroadphaseInterface* pairCache;
	btSequentialImpulseConstraintSolver* solv;
	//std::vector <btRigidBody*> bullets;
	//std::vector <btRigidBody*> players;
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

	void Init(pos_list player_poss, pos_list flag_poss);
	std::shared_ptr<Player> GetPlayer(int id) { return players.at(id); };

	// Updates Physics world by one tick
	// Handles egg collisions with players
	void UpdateWorld();

	// Finds and Removes flag from world list of flags
	void removeFlag(std::shared_ptr<Flag> collectedFlag);

	void SpawnInWorld(int classid, int teamid, int playeridforBullet, int damageforBullet, PosInfo position, btVector3* speed);

};