#pragma once

#include "../../network/GameData.h"
#include <vector>

typedef vector<PosInfo> pos_list;

class World {
public:
	World();
	~World();

	void Init(pos_list player_poss);

private:
	// list of map objects
	// list (vector) of players sorted by client id
	// list of eggs
};