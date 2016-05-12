#pragma once

#ifndef GAME_DATA
#define GAME_DATA

#include <string.h>
#include <stdio.h>
#include "../server/engine/ObjectId.h"

const int WORLD_WIDTH = 100;
const int WORLD_HEIGHT = 100;

static int oid0 = 0;
static int oid1 = 0;
static int oid2 = 0;

enum MoveType {

    BAD_MOVE = -1,

    MOVE_FORWARD = 0,

    MOVE_BACKWARD = 1,

    MOVE_LEFT = 2,

    MOVE_RIGHT = 3
};

enum GameDataId 
{
    POS_OBJ = 0,
	REM_OBJ = 1
};

struct GameInfo
{
    GameDataId id;

    // NETWORKING NOTE:
    // Usually, you want to serialize into PacketData's buf before you send
    void serialize(char * data) {
        memcpy(data, this, sizeof(GameInfo));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(GameInfo));
    }
};

// Position info of object
struct PosInfo : GameInfo
{

	int oid;
	ClassId cid;

	// object coordinates
    float x;
    float y;
    float z;

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

// What needs to get removed
struct RemInfo : GameInfo
{
	int rem_oid;
	ClassId rem_cid;

	void serialize(char * data) {
		memcpy(data, this, sizeof(RemInfo));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(RemInfo));
	}
};

#endif