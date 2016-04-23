#include "engine.h"
#include <vector>
#include <time.h>

const int WORLD_WIDTH = 100;
const int WORLD_HEIGHT = 100;

Engine::Engine() {
	world = new World();
}

Engine::~Engine() {

}

void Engine::InitWorld(int num_players) {
	printf("INITIALIZING WORLD ON SERVER\n");

    pos_list player_poss = GenerateCoords(num_players);
    pos_list egg_poss = GenerateCoords(num_players * 2);
}

pos_list Engine::GenerateCoords(int n) {
    srand (time(NULL));
    pos_list poss;

    for(int i = 0; i < n; i++) {
         PosInfo pos;

        pos.x = rand() % WORLD_WIDTH + 1; // generate random int from 1 to width
        pos.y = rand() % WORLD_HEIGHT + 1;
        pos.z = 0;

        /* face random direction */
		pos.v_rotation = 0;
		pos.h_rotation = 0;
        
        poss.push_back(pos);
    }

    return poss;
}