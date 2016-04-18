#pragma once
#include <stdlib.h>
#include "ServerNetwork.h"
#include "../NetworkData.h"
#include "../DummyWorld.h"

class ServerGame
{

public:

    ServerGame(void);
    ~ServerGame(void);

    void update();

	void receiveFromClients();

	void sendActionPackets();
    
    // Want singleton for the world, if we receive an init packet from a new client
    // we want to send them the current world data, not reset the world
    // Assume 1 client for now
    void receiveInitPacket();
    void sendInitPacket();

    // The data we want in network_data should have an offset if any
    void receiveSpawnPacket(int offset);
    void sendSpawnPacket();


private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

    // dummy world
    DummyWorld* world;

   // The ServerNetwork object 
    ServerNetwork* network;

	// data buffer
   char network_data[MAX_PACKET_SIZE];

};