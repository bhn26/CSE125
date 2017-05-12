#pragma once

#include "EntitySpawner.h"
#include "network/GameData.h"

#include <vector>
#include <memory>
#include <map>

class WorldObstacle;
class MapLoader;

typedef std::vector<PosInfo> pos_list;

class World
{
private:
	// list of game world objects
	std::vector<std::shared_ptr<Flag>> flags;
	// Delete list, mainly for bullets
	std::vector<Entity*> deleteList;  
	// Marked list to protect multi-handling, for flags
	std::vector<Entity*> markedList;
	// List of entities to unmark at the end
	std::vector<Entity*> unmarkList;

	// Physics World attributes
	btDiscreteDynamicsWorld* curWorld;
	btDefaultCollisionConfiguration* colConfig;
	btCollisionDispatcher* disp;
	btBroadphaseInterface* pairCache;
	btSequentialImpulseConstraintSolver* solv;

	// Map objects
	WorldObstacle* ground;
	WorldObstacle* frontWall;
	WorldObstacle* backWall;
	WorldObstacle* leftWall;
	WorldObstacle* rightWall;

	// object ids
	int oid;

	// Map Loader object
	MapLoader* worldMapLoader;

	// list of fields to check.  Explosions and mounts.  Maybe decouple into a class of it's own.  Have a TTL

public:
	World();
	~World();

	// ticker used for now
	unsigned int world_tick = 0;

	void Init();

	void PreSpawn();
	btDiscreteDynamicsWorld* GetPhysicsWorld();

	// Updates Physics world by one tick
	void UpdateWorld();

	// Finds and Removes flag from world list of flags
	void removeFlag(Flag* collectedFlag);

};