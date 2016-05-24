#pragma once

#include <string.h>
#include <stdio.h>
#include "../server/engine/ObjectId.h"

const int WORLD_WIDTH = 50;
const int WORLD_HEIGHT = 50;

/*
static int oid0 = 0;
static int oid1 = 0;
static int oid2 = 0;
*/

static unsigned int currentWorldTick;

enum MoveType {

    BAD_MOVE = -1,

    MOVE_FORWARD = 0,

    MOVE_BACKWARD = 1,

    MOVE_LEFT = 2,

    MOVE_RIGHT = 3
};

enum GameDataId 
{
    POS_OBJ,
	REM_OBJ,
	SCORE_OBJ
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
	// general info
	int id; // client id
	int team_id;

	int skin;

	int oid; // object id
	ClassId cid; // class id

	// object coordinates
    float x;
	float y;
	float z;

    int direction; // client -> server move data

	//rotation coords
	float rotw;
	float rotx;
	float roty;
	float rotz;

	int num_eggs; // num eggs this player has 

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

// team scores
struct ScoreInfo : GameInfo {
	int t0_score;
	int t1_score;

	void serialize(char * data) {
		memcpy(data, this, sizeof(ScoreInfo));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(ScoreInfo));
	}
};


