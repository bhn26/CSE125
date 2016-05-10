#pragma once
#include <string.h>
#include "GameData.h"

#define MAX_PACKET_SIZE 1000000
#define DATA_SIZE 100 // Change this to the largest data size that is needed

enum PacketTypes 
{

    INIT_CONNECTION = 0, // Initial connection setup

	READY_GAME = 1,  // The clients send this to say they're ready

	START_GAME = 2, // A client would send this to start the game

    SPAWN_EVENT = 3, // Creating a new object, sent to the client

	REMOVE_EVENT = 4, // Removing an object, sent to the client

    MOVE_EVENT = 5, //3

    V_ROTATION_EVENT = 6 //4

};

struct PacketHeader
{
    unsigned int packet_type;

    // id for server will be -1
    int sender_id;
    int receiver_id;

};

struct PacketData
{
	GameDataId game_data_id;
    char buf[DATA_SIZE];
};

struct Packet
{

    struct PacketHeader hdr;

    struct PacketData dat;


    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};