
#include "ServerGame.h"

unsigned int ServerGame::client_id; 

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(); 

    world = new DummyWorld();
}

ServerGame::~ServerGame(void)
{
}

void ServerGame::update()
{
    // get new clients
   if(network->acceptNewClient(client_id))
   {
        printf("client %d has been connected to the server\n",client_id);

        client_id++;
   }

   receiveFromClients();
}

void ServerGame::receiveFromClients()
{

    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length) 
        {

            // Deserialize and check the type of packet
            packet.deserialize(&(network_data[i]));
            
            switch (packet.packet_type) {

                case INIT_CONNECTION:

                    receiveInitPacket();
                    sendInitPacket();
                    //sendActionPackets();

                    break;

                case ACTION_EVENT:

                    printf("server received action event packet from client\n");

                    sendActionPackets();

                    break;

                case SPAWN_EVENT:

                    // Receive packet
                    // Check validity of event
                    // If check passes, send update to clients
                    receiveSpawnPacket(i);
                    sendSpawnPacket();
              
                    break;

                default:

                    printf("error in packet types\n");

                    break;
            }
            i += sizeof(Packet);
        }
    }
}


void ServerGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}


void ServerGame::receiveInitPacket()
{
    printf("server received init packet from client\n");
}

// We actually only want to send this back to the client that we received it from, but assume 1 client for now
void ServerGame::sendInitPacket()
{
    printf("sending init to clients\n");
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
}

// Assume one client for now, we're going to need to send client id with each packet probably
// in order to distinguish the owner of the event, right now we don't do this.
void ServerGame::receiveSpawnPacket(int offset)
{
    printf("Received spawn packet from client\n");

    // Check if valid spawn and stuff like that
}


void ServerGame::sendSpawnPacket()
{

    const unsigned int packet_size = sizeof(Packet);
    struct SpawnInfo sp;
    sp.x = rand() % 5;
    sp.y = rand() % 5;
    printf("spawned dummy at (%d,%d)\n", sp.x, sp.y);
    char* packet_data = (char*) &sp;
    
    Packet packet;
    packet.packet_type = SPAWN_EVENT;

    packet.serialize(packet_data);

    printf("size of Packet: %d\n", packet_size);

    network->sendToAll(packet_data, packet_size);

}
