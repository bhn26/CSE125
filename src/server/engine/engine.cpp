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

void Engine::SpawnRandomPlayer(int player, int team, int skin)
{
	PosInfo p;
	p.id = player;
	p.team_id = team;
	p.skin = skin;
	std::pair<int, int> loc = EntitySpawner::getRandomLoc();
	p.x = loc.first;
	p.z = loc.second;
	p.y = 3;
	EntitySpawner::Instance()->spawnPlayer(team, p, world->GetPhysicsWorld());
}

void Engine::SpawnRandomFlag()
{
	PosInfo p;
	std::pair<int, int> loc = EntitySpawner::getRandomLoc();
	p.x = loc.first;
	p.z = loc.second;
	p.y = 3;
	EntitySpawner::Instance()->spawnFlag(p, world->GetPhysicsWorld());
}

int Engine::RandomNum(int mod)
{
	return rand() % mod;
}

void Engine::SendPreSpawn(int n)
{
	// Always spawn the players before other dynamic objects
	world->PreSpawn();

	initialSpawned = true;
}