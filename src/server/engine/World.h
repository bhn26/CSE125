#pragma once

#include "../../network/GameData.h"
#include <vector>
#include <memory>
#include "Player.h"

using namespace std;

typedef vector<PosInfo> pos_list;

class World {
public:
	World();
	~World();

	void Init(pos_list player_poss);
	std::shared_ptr<Player> GetPlayer(int id) { return players.at(id); };

private:
	// list of map objects
	// list (vector) of players sorted by client id
	std::vector<std::shared_ptr<Player>> players;
	// list of eggs
};