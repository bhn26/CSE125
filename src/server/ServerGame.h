#pragma once
#include <stdlib.h>
#include "ServerNetwork.h"
#include "../network/NetworkData.h"
#include "engine/engine.h"
#include <btBulletDynamicsCommon.h>

class ServerGame
{

public:

    ServerGame(void);
    ~ServerGame(void);

    static unsigned int NumClients() {return client_id;}

    void update();

	void receiveFromClients();

	void sendActionPackets();
    
    // Want singleton for the world, if we receive an init packet from a new client
    // we want to send them the current world data, not reset the world
    // Assume 1 client for now
    void receiveInitPacket(int offset);
    void sendInitPacket();

	void receiveStartPacket(int offset);
	void sendStartPacket();

    // The data we want in network_data should have an offset if any
    void receiveSpawnPacket(int offset);
    void sendSpawnPacket();

    // Returns the direction to be moved, if it can't move there, returns BAD_MOVE
    void receiveMovePacket(int offset);
    void sendMovePacket(int client);

    void receiveVRotationPacket(int offset);
    void sendVRotationPacket(int client); 

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

	bool game_started = false;

	Engine * engine;

   // The ServerNetwork object 
    ServerNetwork* network;

	// data buffer
   char network_data[MAX_PACKET_SIZE];

};