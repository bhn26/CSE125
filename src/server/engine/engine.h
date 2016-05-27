
#include "World.h"

#ifndef GAME_DATA
#define GAME_DATA
#include "../../network/GameData.h"
#endif

using namespace std;

typedef vector<PosInfo> pos_list;

class Engine {
public:
	Engine();
	~Engine();

	World * GetWorld() { return world; };
	// generate map
	void InitWorld(int num_players);

	// Spawns at random positions
	void SpawnRandomPlayer(int player, int team, int skin);
	void SpawnRandomFlag();

	int RandomNum(int mod);

	// spawn once all players are "ready"
	void SendPreSpawn(int n);
	bool hasInitialSpawned() { return initialSpawned; };
	void setInitialSpawned(bool b) { initialSpawned = b; }

	// move player
	// attack player

private:
	bool initialSpawned = false;
	World * world; // used to communicate with clients

	/* maybe put the world representation in this class so we can 
	manipulate it and use ServerGame to send packets to clients*/

	pos_list GenerateCoords(int n); // generate random coordinates for n objs
};