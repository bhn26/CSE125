#pragma once
#include "Basic/Singleton.h"

#include "EntitySpawner.h"
#include "Weapon.h"

class CollectableSpawner : public Singleton<CollectableSpawner>
{
    friend class Singleton<CollectableSpawner>;

private:
    CollectableSpawner() = default;
    ~CollectableSpawner() = default;
    unsigned int m_last_spawn = 0;              // the last tick of when a collectable was spawned
    unsigned int m_wait_period = STARTING_TICK; // Wait period before next collectable spawn
    unsigned int m_collectables = 0;            // number of collectables that currently exist

public:
    static const int STARTING_TICK = 25;  // the tick to start spawning collectables on
    static const int MIN_WEAP_SPAWNS = 1; // Minumum number of weapons spawned for this spawn period
    static const int MAX_WEAP_SPAWNS = 3; // Maximum number of weapons spawned for this spawn period
    static const int MIN_POW_SPAWNS = 1; // Minumum number of powerups spawned for this spawn period
    static const int MAX_POW_SPAWNS = 2; // Minumum number of powerups spawned for this spawn period
    static const int MIN_SPAWN_PERIOD = 400; //  The minimum number of ticks before a new collectable spawns
    static const int MAX_SPAWN_PERIOD = 700; //  The maximum number of ticks before a new collectable spawns
    static const int MAX_COLLECTABLES = 60; // Maximum number of collectables that can exist at once

    // The world to spawn the collectable in
    void SpawnRandomCollectables(btDiscreteDynamicsWorld* curworld, int current_tick);

    unsigned int GetCollectables() const { return m_collectables; }
    void DecCollectables() { m_collectables--; }
};