#include "engine.h"
#include <math.h>
#include <vector>

Engine::Engine(ServerGame * g) {
    this.game = g;
}

Engine::~Engine() {

}

void Engine::InitMap() {
    //Scene::WIDTH
    //Scene::HEIGHT
    pos_list player_poss = GenerateCoords(g->NumClients());
    pos_list egg_poss = GenerateCoords(g->NumClients() * 2);
}

pos_list Engine::GenerateCoords(int n) {
    srand (time(NULL));
    pos_list poss;

    for(int i = 0; i < n; i++) {
         PosInfo pos;

        pos.x = rand() % Scene::WIDTH + 1; // generate random int from 1 to width
        pos.y = rand() % Scene::HEIGHT + 1;
        pos.z = 0;

        /* face random direction */
        pos.radians = rand() % 2 * M_PI; // 0 - 359 degrees
        
        poss.push_back(pos);
    }

    return poss;
}