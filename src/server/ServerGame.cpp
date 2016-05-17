
#include "ServerGame.h"
#include "engine\Player.h"

unsigned int ServerGame::client_id; 

ServerGame* ServerGame::sg = nullptr;

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;
	game_started = false;

    // set up the server network to listen 
    network = new ServerNetwork(); 

	engine = new Engine();
}

ServerGame::~ServerGame(void)
{
}


void ServerGame::update()
{
	// get new clients only if the game hasn't started
	if (!game_started)
	{
		if (network->acceptNewClient(client_id))
		{
			printf("client %d has been connected to the server\n", client_id);

			// This will be an INIT_CONNECTION packet
			receiveFromClients();
			client_id++;
		}
	}
	
	receiveFromClients();

	// Check that all clients are ready
	if (game_started && ready_clients == client_id)
	{
		if(!engine->hasInitialSpawned())
			engine->InitialSpawn(ready_clients);
		engine->GetWorld()->UpdateWorld();
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

				case READY_GAME:
					ready_clients++;
					//printf("ready clients: %d\nclient_id: %d\n", ready_clients, client_id);
					break;

				case START_GAME:
					receiveStartPacket(i);
					sendStartPacket();

					break;

                case MOVE_EVENT:
                    // sends a move packet based on if reception was valid
                    receiveMovePacket(i);
                    break;

                case SPAWN_EVENT: // This will probably be deleted, only server spawns things i think

                    // Receive packet
                    // Check validity of event
                    // If check passes, send update to clients
                    receiveSpawnPacket(i);
              
                    break;

                case V_ROTATION_EVENT:
                    receiveRotationPacket(i + sizeof(PacketHeader));
                
                    break;

                default:

                    printf("error in packet types\n");

                    break;
            }
            i += sizeof(Packet);
        }
    }
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

    // Send back to the client and assign client id
    packet.hdr.receiver_id = client_id;
    packet.hdr.sender_id = SERVER_ID;

    packet.serialize(packet_data);

    network->sendToClient(packet_data, packet_size, client_id);
    printf("server sent init packet to client %d\n", client_id);

}

// Unused, we send the init back right away in receive
// We actually only want to send this back to the client that we received it from, but assume 1 client for now
void ServerGame::sendInitPacket()
{
    printf("sending init to clients\n");
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

	network->sendToClient(packet_data, packet_size, client_id);
}

void ServerGame::receiveStartPacket(int offset) {
	struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);

	printf("recieved start packet from %d\n", hdr->sender_id);
	if (!game_started) {
		printf("initializing world with %d players", client_id + 1);
		engine->InitWorld(client_id + 1);
		game_started = true;
	}
	/*else {
		printf("re-initializing world with %d players", client_id + 1);
		engine->InitWorld(client_id + 1);
	}*/

	// add player
}

void ServerGame::sendStartPacket() { // will add more later based on generated world
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.hdr.packet_type = START_GAME;


    PosInfo p;
    //p.id = client_id + 1;

    packet.dat.game_data_id = POS_OBJ;

	printf("sending start packet with client_id %d\n", client_id + 1);

    p.serialize(packet.dat.buf);
    packet.serialize(packet_data);

    packet.hdr.sender_id = SERVER_ID;

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


void ServerGame::sendSpawnPacket(PosInfo pi)
{
    Packet packet;
    packet.hdr.packet_type = SPAWN_EVENT;

    const unsigned int packet_size = sizeof(Packet);

    char packet_data[packet_size];

    packet.dat.game_data_id = POS_OBJ;
	pi.serialize(packet.dat.buf);

    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);

}

void ServerGame::sendRemovePacket(ClassId cid, int oid)
{
	Packet packet;
	packet.hdr.packet_type = REMOVE_EVENT;

	const unsigned int packet_size = sizeof(Packet);

	char packet_data[packet_size];

	packet.dat.game_data_id = REM_OBJ;

	RemInfo r;
	r.rem_cid = cid;
	r.rem_oid = oid;

	r.serialize(packet.dat.buf);

	packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}

void ServerGame::receiveMovePacket(int offset)
{

	struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);
	printf("recieved move packet from %d\n", hdr->sender_id);
    struct PacketData* dat = (struct PacketData *) &(network_data[offset + sizeof(PacketHeader)]);
    struct PosInfo* pi = (struct PosInfo *) &(dat->buf);
	shared_ptr<Player> player = engine->GetWorld()->GetPlayer(hdr->sender_id);

    //printf("dummy's current pos is (%d,%d)\n", dpi.x, dpi.y);
	btVector3* vec;
	switch (pi->direction) {
	case MOVE_FORWARD:
		vec = new btVector3(0, 0, -3);
		player->Move(vec);
		delete vec;
		break;
	case MOVE_BACKWARD:
		vec = new btVector3(0, 0, 3);
		player->Move(vec);
		delete vec;
		break;
	case MOVE_LEFT:
		vec = new btVector3(-3, 0, 0);
		player->Move(vec);
		delete vec;
		break;
	case MOVE_RIGHT:
		vec = new btVector3(3, 0, 0);
		player->Move(vec);
		delete vec;
		break;
	}

//	player->Move(pi->direction);
//	sendMovePacket(hdr->sender_id);
}

void ServerGame::sendMovePacket(ClassId class_id, int obj_id)
{
		shared_ptr<Player> player = engine->GetWorld()->GetPlayer(obj_id); // change this to getting the object
        Packet packet;
		packet.hdr.sender_id = SERVER_ID;
        packet.hdr.packet_type = MOVE_EVENT;

        PosInfo p;
        packet.dat.game_data_id = POS_OBJ;
	
		// Extract the vector and send it with the posinfo object
		btVector3 vec = player->GetEntityPosition();

		p = player->GetPosition();
		p.cid = class_id;
		p.oid = obj_id;
		p.x = vec.getX();
		p.y = vec.getY();
		p.z = vec.getZ();

        p.serialize(packet.dat.buf);

        const unsigned int packet_size = sizeof(Packet);
        char packet_data[packet_size];

        packet.serialize(packet_data);

        network->sendToAll(packet_data, packet_size);
        //printf("Sent move packet to clients\n");
}

void ServerGame::receiveRotationPacket(int offset) {

    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* pi = (struct PosInfo *) &(dat->buf);

	struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset - sizeof(PacketHeader)]);

	shared_ptr<Player> player = engine->GetWorld()->GetPlayer(hdr->sender_id);

    // TODO - rotate player in game state
    //player->Rotate(pi->v_rotation, pi->h_rotation);

	//sendVRotationPacket(hdr->sender_id);
}

void ServerGame::sendRotationPacket(int client) {
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

	shared_ptr<Player> player = engine->GetWorld()->GetPlayer(client);
    Packet packet;
	packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = V_ROTATION_EVENT;

    packet.dat.game_data_id = POS_OBJ;

	PosInfo p = player->GetPosition();
	p.oid = client;

    p.serialize(packet.dat.buf);
    
    packet.serialize(packet_data);

	//network->sendToAll(packet_data, packet_size);
}
