#include "engine.h"
#include "EntitySpawner.h"
#include <vector>
#include <time.h>

Engine::Engine() {
	world = new World();
}

Engine::~Engine() {

}

void Engine::InitWorld(int num_players) {
	printf("INITIALIZING WORLD ON SERVER\n");

	// we only need to seed once
	srand(time(NULL));
    //pos_list player_poss = GenerateCoords(num_players);
    //pos_list flag_poss = GenerateCoords(num_players * 2);

	world->Init();
}

void Engine::SpawnRandomPlayer()
{
	PosInfo p;
	p.x = rand() % WORLD_WIDTH + 1;
	p.z = rand() % WORLD_WIDTH + 1;
	p.y = 3;
	int teamid = rand() % 2;
	EntitySpawner::instance()->spawnPlayer(teamid, p, world->GetPhysicsWorld());
}

void Engine::SpawnRandomFlag()
{
	PosInfo p;
	p.x = rand() % WORLD_WIDTH + 1;
	p.z = rand() % WORLD_WIDTH + 1;
	p.y = 3;
	int teamid = rand() % 2;
	EntitySpawner::instance()->spawnFlag(p, world->GetPhysicsWorld());
}

void Engine::InitialSpawn(int n)
{
	// Always spawn the players before other dynamic objects
	for ( int i = 0; i < n; i++)
	{
		SpawnRandomPlayer();
	}
	for (int i = 0; i < 2 * n; i++)
	{
		SpawnRandomFlag();
	}
	initialSpawned = true;
}

pos_list Engine::GenerateCoords(int n) {
    pos_list poss;

    for(int i = 0; i < n; i++) {
         PosInfo pos;

        pos.x = rand() % WORLD_WIDTH + 1; // generate random int from 1 to width
        pos.z = rand() % WORLD_WIDTH + 1;

		// y is the vertical axis and should be 0, not randomized
        pos.y = 3;

        /* face random direction */
		//pos.v_rotation = 0;
		//pos.h_rotation = 0;
        
        poss.push_back(pos);
    }

    return poss;
}