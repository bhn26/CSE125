#pragma once
#include "EntitySpawner.h"

static const int NUM_WEAPON_TYPES = 1;

enum CollectableType {
	SEEDGUN
};

class CollectableSpawner {

public:
	// The world to spawn the collectable in
	void SpawnRandomCollectable(btDiscreteDynamicsWorld* curworld);

};