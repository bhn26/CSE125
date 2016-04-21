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
    int x;
    int y;
    int z;
    int direction;
};