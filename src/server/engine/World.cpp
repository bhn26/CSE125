#include "World.h"

World::World() {
	// initialize map objects 
}

World::~World() {

}

void World::Init(pos_list player_poss) {
	for (int i = 0; i < player_poss.size(); i++) {
		std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(i, player_poss.at(i)));
		players.push_back(player);
	}
}