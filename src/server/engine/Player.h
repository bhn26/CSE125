#pragma once
#include "../../network/GameData.h"

class Player {
public:
	Player(int id, PosInfo pos);
	~Player();

private:
	int id;
	// team
	PosInfo position;
};