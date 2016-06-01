#pragma once
#include "EntitySpawner.h"

class CollectableSpawner {

private:
	CollectableSpawner() { last_spawn = 0; wait_period = STARTING_TICK; }
	~CollectableSpawner() {}
	unsigned int last_spawn; // the last tick of when a collectable was spawned
	unsigned int wait_period; // Wait period before next collectable spawn

public:
	static const int NUM_WEAPON_TYPES = 1; // number of types of weapons there are
	static const int STARTING_TICK = 100;  // the tick to start spawning collectables on
	static const int MIN_NUM_SPAWNS = 2;   // Minumum number of collectables spawned for this spawn period
	static const int MAX_NUM_SPAWNS = 4;   // Maximum number of collectables spawned for this spawn period
	static const int MIN_SPAWN_PERIOD = 400;   //  The minimum number of ticks before a new collectable spawns
	static const int MAX_SPAWN_PERIOD = 700;   //  The maximum number of ticks before a new collectable spawns

	static CollectableSpawner* instance()
	{
		static CollectableSpawner* instance = new CollectableSpawner();
		return instance;
	}

	// The world to spawn the collectable in
	void SpawnRandomCollectable(btDiscreteDynamicsWorld* curworld, int current_tick);

};