#pragma once
#include "../../network/GameData.h"

class Player {
public:
	Player(int id, PosInfo pos);
	~Player();

	PosInfo GetPosition() { return position; };
	void Move(int direction);
	void Rotate(float v_rotation, float h_rotation);

private:
	int id;
	// team
	PosInfo position;
};