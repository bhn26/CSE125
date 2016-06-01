#include "CollectableSpawner.h"
#include "Weapon.h"

void CollectableSpawner::SpawnRandomCollectables(btDiscreteDynamicsWorld* curworld, int current_tick)
{
	// determine whether or not we should spawn
	if (last_spawn + wait_period != current_tick || collectables == MAX_COLLECTABLES)
		return;

	// determine how many to spawn
	int num_spawns = (rand() % (MAX_NUM_SPAWNS + MIN_NUM_SPAWNS)) + MIN_NUM_SPAWNS;

	for(int i = 0; i < num_spawns; i++)
	{
		// determine which weapon and spawn the number of weapons
		EntitySpawner::instance()->spawnCollectable(curworld, (WeaponType) (rand() % NUM_WEAPON_TYPES));
		collectables++;

		if (collectables == MAX_COLLECTABLES)
			break;
	}

	// schedule the next spawn
	last_spawn = current_tick;
	wait_period = (rand() % (MAX_SPAWN_PERIOD - MIN_SPAWN_PERIOD)) + MIN_SPAWN_PERIOD;

}