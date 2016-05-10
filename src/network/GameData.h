#pragma once

//#ifndef GAME_DATA
//#define GAME_DATA

#include <string.h>
#include <stdio.h>

const int WORLD_WIDTH = 100;
const int WORLD_HEIGHT = 100;

enum MoveType {

    BAD_MOVE = -1,

    MOVE_FORWARD = 0,

    MOVE_BACKWARD = 1,

    MOVE_LEFT = 2,

    MOVE_RIGHT = 3
};

enum ObjId 
{
    POS_OBJ = 0
};

struct GameInfo
{
    ObjId id;

    // NETWORKING NOTE:
    // Usually, you want to serialize into PacketData's buf before you send
    void serialize(char * data) {
        memcpy(data, this, sizeof(GameInfo));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(GameInfo));
    }
};

struct PosInfo : GameInfo
{

	int id;
	// object coordinates

    int x;
    int y;
    int z;

    int direction; // remove later?

    // rotation
    float v_rotation;
    float h_rotation;

    void serialize(char * data) {
        memcpy(data, this, sizeof(PosInfo));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(PosInfo));
    }
};


//#endif