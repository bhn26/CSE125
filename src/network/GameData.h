#pragma once
#include <string.h>
#include <stdio.h>
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
<<<<<<< 2dce00a48b1270158a0c13e20220c3b340a84734
<<<<<<< 849bab3ad8a94e6e1f5e449c2f13e0b7c0efe664
=======
	int id;
	// object coordinates
>>>>>>> multiplayer WIP
=======
    int id;

	// object coordinates

>>>>>>> now using a bytebuffer for packets
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
