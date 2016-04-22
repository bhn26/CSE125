
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

        // This will be an INIT_CONNECTION packet
        receiveFromClients();
        client_id++;
   }
   else
   {
       receiveFromClients();
   }
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
            
            switch (packet.hdr.packet_type) {

                case INIT_CONNECTION:

                    // offset is i here since we want to parse the packet header
                    // receive it and send it right back
                    receiveInitPacket(i);
                    //sendInitPacket();
                    //sendActionPackets();

                    break;

                case ACTION_EVENT:

                    printf("server received action event packet from client\n");

                    sendActionPackets();

                    break;

                case MOVE_EVENT:
                    // sends a move packet based on if reception was valid
                    sendMovePacket(receiveMovePacket(i + sizeof(PacketHeader)));
                    break;

                case SPAWN_EVENT:

                    // Receive packet
                    // Check validity of event
                    // If check passes, send update to clients
                    receiveSpawnPacket(i);
                    sendSpawnPacket();
              
                    break;

                case V_ROTATION_EVENT:
                    receiveVRotationPacket(i + sizeof(PacketHeader));
                    sendVRotationPacket();
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
    packet.hdr.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}

// Handle new init packet from client
void ServerGame::receiveInitPacket(int offset)
{
    struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);
    
    printf("server received init packet from client\n");
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = INIT_CONNECTION;

    // Send back to the client
    packet.hdr.receiver_id = client_id;
    packet.hdr.sender_id = SERVER_ID;

    packet.serialize(packet_data);

    network->sendToClient(packet_data, packet_size, client_id);
    printf("server sent init packet to client %d\n", client_id);

}

// We actually only want to send this back to the client that we received it from, but assume 1 client for now
void ServerGame::sendInitPacket()
{
    printf("sending init to clients\n");
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
}

// Assume one client for now, we're going to need to send client id with each packet probably
// in order to distinguish the owner of the event, right now we don't do this.
void ServerGame::receiveSpawnPacket(int offset)
{
    struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);
    
    printf("Received spawn packet from client %d\n", hdr->sender_id);

    // Check if valid spawn and stuff like that
}


void ServerGame::sendSpawnPacket()
{
    Packet packet;
    packet.hdr.packet_type = SPAWN_EVENT;

    const unsigned int packet_size = sizeof(Packet);

    packet.pi.x = rand() % 5;
    packet.pi.y = rand() % 5;

    printf("spawned dummy at (%d,%d)\n", packet.pi.x, packet.pi.y);
    world->spawnDummy(packet.pi.x, packet.pi.y);
    char packet_data[packet_size];


    packet.serialize(packet_data);

    //printf("size of Packet: %d\n", packet_size);

    network->sendToAll(packet_data, packet_size);

}

int ServerGame::receiveMovePacket(int offset)
{
    struct PosInfo* pi = (struct PosInfo *) &(network_data[offset]);

    struct PosInfo dpi = world->getDummyPos();
    //printf("dummy's current pos is (%d,%d)\n", dpi.x, dpi.y);

    // check if this is a valid move
    if (world->canMove(pi->direction))
    {
        world->moveDummy(pi->direction);
        dpi = world->getDummyPos();
        printf("Dummy moved to (%d,%d)\n", dpi.x, dpi.y);
        return pi->direction;
    }
    else
    {
        dpi = world->getDummyPos();
        printf("dummy cannot move past (%d,%d)\n", dpi.x, dpi.y);
    }
    return BAD_MOVE;

}

void ServerGame::sendMovePacket(int direction)
{
    // don't send anything if it's a bad move
    if (direction != BAD_MOVE)
    {
        Packet packet;
        packet.hdr.packet_type = MOVE_EVENT;
        packet.pi.direction = direction;

        const unsigned int packet_size = sizeof(Packet);
        char packet_data[packet_size];

        packet.serialize(packet_data);

        network->sendToAll(packet_data, packet_size);
        //printf("Sent move packet to clients\n");
    }
}

void ServerGame::receiveVRotationPacket(int offset) {
    struct PosInfo* pi = (struct PosInfo *) &(network_data[offset]);

	printf("rotating player by %d\n", pi->radians);
    // TODO - rotate player in game state
    world->rotateDummy(pi->radians);
}

void ServerGame::sendVRotationPacket() {
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = V_ROTATION_EVENT;
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    packet.pi = world->getDummyRotation();

	printf("send rotate packet by %d\n", packet.pi.radians);

    packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}
