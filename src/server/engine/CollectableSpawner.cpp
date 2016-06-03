#include "CollectableSpawner.h"
#include "Weapon.h"

void CollectableSpawner::SpawnRandomCollectables(btDiscreteDynamicsWorld* curworld, int current_tick)
{
	// determine whether or not we should spawn
	if (last_spawn + wait_period != current_tick || collectables == MAX_COLLECTABLES)
		return;

	// determine how many to spawn
	int num_spawns = (rand() % (MAX_NUM_SPAWNS + MIN_NUM_SPAWNS)) + MIN_NUM_SPAWNS;

	int num_weapons = 0;
	int num_powers = num_spawns;

	// spawn that number of weapons
	for(int i = 0; i < num_weapons; i++)
	{
		// determine which weapon and spawn the number of weapons
		EntitySpawner::instance()->spawnCollectable(curworld, (WeaponType) (rand() % NUM_WEAPON_TYPES));
		collectables++;

		if (collectables == MAX_COLLECTABLES)
			break;
	}

	for (int i = 0; i < num_powers; i++)
	{
		// determine which weapon and spawn the number of weapons
		EntitySpawner::instance()->spawnCollectable(curworld, (PowerupType)(rand() % NUM_POWER_TYPES));
		collectables++;

		if (collectables == MAX_COLLECTABLES)
			break;
	}

	// schedule the next spawn
	last_spawn = current_tick;
	wait_period = (rand() % (MAX_SPAWN_PERIOD - MIN_SPAWN_PERIOD)) + MIN_SPAWN_PERIOD;

}