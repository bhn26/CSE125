#pragma once
#include <string.h>
#include "GameData.h"

#define MAX_PACKET_SIZE 1000000

enum PacketTypes 
{

    INIT_CONNECTION = 0, //0

	START_GAME = 1, //1

    ACTION_EVENT = 2, //2

    SPAWN_EVENT = 3, //3

    MOVE_EVENT = 4, //4

    V_ROTATION_EVENT = 5 //5

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