#pragma once
enum MoveType {

    BAD_MOVE = -1,

    MOVE_FORWARD = 0,

    MOVE_BACKWARD = 1,

    MOVE_LEFT = 2,

    MOVE_RIGHT = 3
};

struct PosInfo
{
	// object coordinates
    int x;
    int y;
    int z;

    int direction; // remove later?

    // rotation
    float v_rotation;
	float h_rotation;
};