#pragma once
#include <string.h>
#include "GameData.h"

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

    SPAWN_EVENT = 2,

    MOVE_EVENT = 3

};

struct Packet {

    unsigned int packet_type;

    struct SpawnInfo sp;

    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};