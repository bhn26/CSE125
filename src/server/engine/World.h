#pragma once

#include "Player.h"
#include "Flag.h"
#include <vector>
#include <memory>

#ifndef GAME_DATA
#define GAME_DATA
#include "../../network/GameData.h"
#endif

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
	btRigidBody * ground;
	btRigidBody * frontWall;
	btRigidBody * backWall;
	btRigidBody * leftWall;
	btRigidBody * rightWall;
	int indexCounter = 0;

public:
	World();
	~World();

	int x = 0;
	int y = 0;

	void Init(pos_list player_poss, pos_list flag_poss);
	std::shared_ptr<Player> GetPlayer(int id) { return players.at(id); };

	// Updates Physics world by one tick
	// Handles egg collisions with players
	void updateWorld();

	// Finds and Removes flag from world list of flags
	void removeFlag(std::shared_ptr<Flag> collectedFlag);

};