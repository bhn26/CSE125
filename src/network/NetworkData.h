#pragma once
#include <string.h>
#include "GameData.h"

#define MAX_PACKET_SIZE 1000000

enum PacketTypes 
{

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

    SPAWN_EVENT = 2,

    MOVE_EVENT = 3

};

struct PacketHeader
{
    unsigned int packet_type;

    // id for server will be -1
    int sender_id;
    int receiver_id;

};

struct Packet
{

    struct PacketHeader hdr;

    struct PosInfo pi;

    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};