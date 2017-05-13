#include "ServerGame.h"
#include "engine/Player.h"

#include <algorithm>

unsigned int ServerGame::m_clientId;

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    m_clientId = 0;
    m_gameStarted = false;

    m_scores[0] = 0;
    m_scores[1] = 0;

    // set up the server m_network to listen
    m_network = new ServerNetwork();

    m_engine = new Engine();
}

ServerGame::~ServerGame(void)
{
}

void ServerGame::update()
{
    // get new clients only if the game hasn't started
    if (!m_gameStarted)
    {
        if (m_network->acceptNewClient(m_clientId))
        {
            printf("client %d has been connected to the server\n", m_clientId);

            // This will be an INIT_CONNECTION packet
            receiveFromClients();
        }
    }

    auto curr_time = std::chrono::steady_clock::now();

    auto fp_stamp = curr_time - m_startTime;

    int diff = fp_stamp.count();

    if (!m_gameOver && m_eggsSpawned && diff >= 300000)
    {
        m_gameOver = true;
        if (m_scores[0] == m_scores[1])
        {
            sendGameOverPacket(-1);
        }
        else if (m_scores[0] > m_scores[1])
        {
            sendGameOverPacket(0);
        }
        else
        {
            sendGameOverPacket(1);
        }
    }
    else
    {
        if (m_gameOver == false && m_eggsSpawned && ((diff % 10000) <= 16))
        {
            sendTimeStampPacket(diff);
        }
    }

    receiveFromClients();

    // Check that all clients are ready

    if (m_gameStarted && m_readyClients >= m_clientId)
    {
        if (m_gameOver)
            return;

        if (m_spawnedClients == m_readyClients && !m_eggsSpawned)
        {
            m_totalEggs = 2 * m_readyClients + 1;
            for (int i = 0; i < m_totalEggs; i++)
            {
                m_engine->SpawnRandomFlag();
            }
            m_eggsSpawned = true;
            Sleep(2000); // should wait for clients to respond
            m_startTime = std::chrono::steady_clock::now();
        }
        if (!m_engine->hasInitialSpawned())
            m_engine->SendPreSpawn(m_readyClients);

        // once eggs has spawned, everything has spawned and we can begin the world cycle
        auto t1 = std::chrono::steady_clock::now();

        if (m_eggsSpawned)
        {
            m_engine->GetWorld()->UpdateWorld();
        }

        auto t2 = std::chrono::steady_clock::now();

        float thresh = 16.67f; // 60 FPS
        //float thresh = 33;   // 30 frames
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        //("DIFFERENCE: %f\n", fp_ms.count());

        if (thresh - fp_ms.count() < 0)
        {
            printf("TIMING ERROR: %f\n", thresh - fp_ms.count());
        }
        else
        {
            // printf("SLEEPING: %f\n", (thresh - fp_ms.count()));

            Sleep((thresh - fp_ms.count()));

            auto t3 = std::chrono::steady_clock::now();

            std::chrono::duration<double, std::milli> fp_after = t3 - t1;

            //("TOTAL AFTER SLEEP: %f\n", fp_after.count());
        }
    }
}

void ServerGame::receiveFromClients()
{
    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for (iter = m_network->m_sessions.begin(); iter != m_network->m_sessions.end(); iter++)
    {
        int data_length = m_network->receiveData(iter->first, m_networkData);

        if (data_length <= 0)
        {
            // no data recieved
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length)
        {
            // Deserialize and check the type of packet
            packet.Deserialize(&m_networkData[i]);

            switch (static_cast<PacketTypes>(packet.hdr.packet_type))
            {
            case PacketTypes::InitConnection:
                // offset is i here since we want to parse the packet header
                // receive it and send it right back
                receiveInitPacket(i);
                // sendInitPacket();
                // sendActionPackets();
                break;
            case PacketTypes::SetUsername:
                receiveNamePacket(i);
                break;
            case PacketTypes::JoinTeam:
                receiveJoinPacket(i);
                break;
            case PacketTypes::ReadyGame:
                m_readyClients++;
                // printf("ready clients: %d\nm_clientId: %d\n", m_readyClients, m_clientId);
                break;
            case PacketTypes::SpawnEvent:
                receiveIndSpawnPacket(i + sizeof(PacketHeader));
                m_spawnedClients++;
                break;
            case PacketTypes::StartGame:
                receiveStartPacket(i);
                Sleep(7000);
                sendStartPacket();
                break;
            case PacketTypes::MoveEvent:
                // sends a move packet based on if reception was valid
                receiveMovePacket(i);
                break;
            case PacketTypes::JumpEvent:
                receiveJumpPacket(i);
                break;
            case PacketTypes::DanceEvent:
                receiveDancePacket(i);
                break;
            case PacketTypes::VRotationEvent:
                receiveRotationPacket(i + sizeof(PacketHeader));
                break;
            case PacketTypes::AttackEvent:
                receiveAttackPacket(i);
                break;
            case PacketTypes::DiscardEvent:
                receiveDiscardPacket(i);
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
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);

    printf("server received init packet from client\n");
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::InitConnection);

    // Send back to the client and assign client id
    packet.hdr.receiver_id = m_clientId;
    packet.hdr.sender_id = SERVER_ID;

    packet.Serialize(packetData);

    m_network->sendToClient(packetData, packetSize, m_clientId);
    printf("server sent init packet to client %d\n", m_clientId);
    m_clientId++;
}

// Unused, we send the init back right away in receive
// We actually only want to send this back to the client that we received it from, but assume 1
// client for now
void ServerGame::sendInitPacket()
{
    printf("sending init to clients\n");
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::InitConnection);

    packet.Serialize(packetData);

    m_network->sendToClient(packetData, packetSize, m_clientId);
}

void ServerGame::receiveJoinPacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);

    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset + sizeof(PacketHeader)]);
    PosInfo* pi = reinterpret_cast<PosInfo*>(&dat->buf);

    printf("recieved join packet from %d for %d\n", hdr->sender_id, pi->team_id);

    int client = hdr->sender_id;

    if (m_teamMap.find(client) == m_teamMap.end())
    { // if new client send all client team data
        for (std::map<int, int>::iterator it = m_teamMap.begin(); it != m_teamMap.end(); it++)
        {
            sendJoinPacket(it->first);
        }
    }

    printf("(before) m_teamMap size = %d", m_teamMap.size());
    m_teamMap[client] = pi->team_id;
    printf("m_teamMap size = %d", m_teamMap.size());
    sendJoinPacket(client);
};

void ServerGame::sendJoinPacket(int client)
{
    const unsigned int packetSize = sizeof(Packet);

    // found
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::JoinTeam);

    PosInfo p;
    p.id = client;
    p.team_id = m_teamMap[client];

    printf("sending join packet for client %d in team %d\n", client, p.team_id);

    packet.dat.game_data_id = GameDataId::Position;

    p.Serialize(packet.dat.buf);
    packet.Serialize(packetData);

    packet.hdr.sender_id = SERVER_ID;

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
};

void ServerGame::receiveStartPacket(int offset)
{
    sendLoadPacket();

    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);

    printf("recieved start packet from %d\n", hdr->sender_id);
    if (!m_gameStarted)
    {
        printf("initializing world with %d players\n", m_clientId + 1);
        m_engine->InitWorld(m_clientId + 1);
        m_gameStarted = true;
    }
    /*else {
        printf("re-initializing world with %d players\n", m_clientId + 1);
        m_engine->InitWorld(m_clientId + 1);
    }*/

    // add player
}

void ServerGame::sendStartPacket()
{ // will add more later based on generated world
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::StartGame);

    PosInfo p;
    // p.id = m_clientId + 1;

    packet.dat.game_data_id = GameDataId::Position;

    printf("sending start packet with m_clientId %d\n", m_clientId + 1);

    p.Serialize(packet.dat.buf);
    packet.Serialize(packetData);

    packet.hdr.sender_id = SERVER_ID;

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendLoadPacket()
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::ServerLoading);

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendReadyToSpawnPacket()
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::ReadyToSpawnEvent);

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::receiveIndSpawnPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    PosInfo* pi = reinterpret_cast<PosInfo*>(&dat->buf);

    m_engine->SpawnRandomPlayer(pi->id, pi->team_id, pi->skin);
}

void ServerGame::sendSpawnPacket(PosInfo pi)
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::SpawnEvent);

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    packet.dat.game_data_id = GameDataId::Position;
    pi.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendRemovePacket(ClassId rem_cid, int rem_oid)
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::RemoveEvent);

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    packet.dat.game_data_id = GameDataId::Remove;

    RemInfo r;
    r.rem_cid = rem_cid;
    r.rem_oid = rem_oid;

    r.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendRemovePacket(ClassId rem_cid, int rem_oid, ClassId rec_cid, int rec_oid)
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::RemoveEvent);

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    packet.dat.game_data_id = GameDataId::Remove;

    RemInfo r;
    r.rem_cid = rem_cid;
    r.rem_oid = rem_oid;
    r.rec_cid = rec_cid;
    r.rec_oid = rec_oid;

    r.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendDiscardPacket(int id)
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::DiscardEvent);

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    MiscInfo m;
    m.misc1 = id;

    m.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendRemovePacket(ClassId rem_cid,
                                  int rem_oid,
                                  ClassId rec_cid,
                                  int rec_oid,
                                  CollectType c_type,
                                  int subtype)
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::RemoveEvent);

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    packet.dat.game_data_id = GameDataId::Remove;

    RemInfo r;
    r.rem_cid = rem_cid;
    r.rem_oid = rem_oid;
    r.rec_cid = rec_cid;
    r.rec_oid = rec_oid;
    r.sub_id = static_cast<int>(c_type);
    r.sub_id2 = subtype;

    r.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::receiveMovePacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset + sizeof(PacketHeader)]);
    PosInfo* pi = reinterpret_cast<PosInfo*>(&dat->buf);
    Entity* ent = reinterpret_cast<Entity*>(EntitySpawner::instance()->GetEntity(ClassId::Player, hdr->sender_id));

    Player* player =
        reinterpret_cast<Player*>(EntitySpawner::instance()->GetEntity(ClassId::Player, hdr->sender_id));

    // don't take client input if stunned
    if (player->GetStun() > 0)
        return;

    switch (static_cast<MoveType>(pi->direction))
    {
    case MoveType::Forward:
    {
        btVector3 curVec = player->GetEntityVelocity();
        curVec.setZ(player->GetPlayerSpeed());
        curVec.setX(0);
        ent->Move(&curVec);
        break;
    }
    case MoveType::Backward:
    {
        btVector3 curVec = player->GetEntityVelocity();
        curVec.setZ(-player->GetPlayerSpeed());
        curVec.setX(0);
        ent->Move(&curVec);
        break;
    }
    case MoveType::Left:
    {
        btVector3 curVec = player->GetEntityVelocity();
        curVec.setX(player->GetPlayerSpeed());
        curVec.setZ(0);
        ent->Move(&curVec);
        break;
    }
    case MoveType::Right:
    {
        btVector3 curVec = player->GetEntityVelocity();
        curVec.setX(-player->GetPlayerSpeed());
        curVec.setZ(0);
        ent->Move(&curVec);
        break;
    }
    }
}

void ServerGame::sendMovePacket(ClassId class_id, int obj_id)
{
    Entity* ent = reinterpret_cast<Entity*>(EntitySpawner::instance()->GetEntity(class_id, obj_id));

    // If this is not a player and the player is dead, don't update
    if (class_id == ClassId::Player && !((Player*)ent)->IsAlive())
    {
        return;
    }

    Packet packet;
    packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::MoveEvent);

    PosInfo p;
    packet.dat.game_data_id = GameDataId::Position;

    // Extract the vector and send it with the posinfo object
    btVector3 vec = ent->GetEntityPosition();

    p.cid = class_id;
    p.oid = obj_id;
    p.x = vec.getX();
    p.y = vec.getY();
    p.z = vec.getZ();

    if (class_id == ClassId::Player)
    {
        p.num_eggs = ((Player*)ent)->GetScore();
        p.jump = ((Player*)ent)->GetJumpSem();
        p.hp = ((Player*)ent)->GetHp();
    }

    p.Serialize(packet.dat.buf);

    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::receiveRotationPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    PosInfo* pi = reinterpret_cast<PosInfo*>(&dat->buf);

    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset - sizeof(PacketHeader)]);

    // All rotation packets will be player type, since it's from client
    Entity* ent = reinterpret_cast<Entity*>(EntitySpawner::instance()->GetEntity(ClassId::Player, hdr->sender_id));

    ent->SetEntityRotation(0, pi->roty, 0, pi->rotw);
    sendRotationPacket(ClassId::Player, hdr->sender_id);
}

void ServerGame::sendRotationPacket(ClassId class_id, int obj_id)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Entity* ent = reinterpret_cast<Entity*>(EntitySpawner::instance()->GetEntity(class_id, obj_id));

    // If this is not a player and the player is dead, don't update
    if (class_id == ClassId::Player && !((Player*)ent)->IsAlive())
    {
        return;
    }

    Packet packet;
    packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::VRotationEvent);

    packet.dat.game_data_id = GameDataId::Position;

    PosInfo p;
    p.cid = (ClassId)class_id;
    p.oid = obj_id;

    // why do we switch w and y and multiply by negative, we don't know but it fixes it
    btQuaternion q = ent->GetEntityRotation();
    btVector3 v = ent->GetEntityPosition();
    p.x = v.getX();
    p.y = v.getY();
    p.z = v.getZ();
    p.rotw = q.getW();
    p.rotx = q.getX();
    p.roty = q.getY();
    p.rotz = q.getZ();

    p.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::receiveJumpPacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);

    Player* player =
        reinterpret_cast<Player*>(EntitySpawner::instance()->GetEntity(ClassId::Player, hdr->sender_id));

    // ignore client input if stunned
    if (player->GetStun() > 0)
        return;

    player->JumpPlayer();
}

void ServerGame::sendScorePacket()
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::UpdateScore);

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    packet.dat.game_data_id = GameDataId::Score;

    ScoreInfo s;
    s.t0_score = m_scores[0];
    s.t1_score = m_scores[1];

    printf("sending score packet: %d, %d\n", s.t0_score, s.t1_score);
    s.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendGameOverPacket(int winner)
{
    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::GameOver);
    m_gameOver = true;

    const unsigned int packetSize = sizeof(Packet);

    std::uint8_t packetData[packetSize];

    packet.dat.game_data_id = GameDataId::Score;

    ScoreInfo s;
    s.t0_score = m_scores[0];
    s.t1_score = m_scores[1];

    printf("sending game over\n");
    s.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::receiveAttackPacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset + sizeof(PacketHeader)]);
    MiscInfo* m = reinterpret_cast<MiscInfo*>(&dat->buf);
    Player* player =
        reinterpret_cast<Player*>(EntitySpawner::instance()->GetEntity(ClassId::Player, hdr->sender_id));

    player->SetCamAngle(m->misc3);

    if (static_cast<AttackType>(m->misc1) == AttackType::Weapon)
    {
        player->UseWeapon();
    }
    else if (static_cast<AttackType>(m->misc1) == AttackType::Peck)
    {
        player->UsePeck();
    }
}

void ServerGame::sendTimeStampPacket(int diff)
{
    Packet packet;
    packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::TimeEvent);

    MiscInfo m;
    m.misc1 = (diff / 1000);

    m.Serialize(packet.dat.buf);

    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    packet.Serialize(packetData);

    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendAttackPacket(int id)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::AttackEvent);

    packet.dat.game_data_id = GameDataId::Emote;

    EmoteInfo e;
    e.id = id;
    e.Serialize(packet.dat.buf);

    packet.Serialize(packetData);
    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::receiveDiscardPacket(int offset)
{
    // PacketHeader* hdr = (PacketHeader *) &(m_networkData[offset]);

    // shared_ptr<Player> player = m_engine->GetWorld()->GetPlayer(hdr->sender_id);
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);
    Player* player =
        reinterpret_cast<Player*>(EntitySpawner::instance()->GetEntity(ClassId::Player, hdr->sender_id));
    player->DiscardWeapon();
}

void ServerGame::receiveDancePacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);
    sendDancePacket(hdr->sender_id);
}

void ServerGame::sendDancePacket(int id)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::DanceEvent);

    packet.dat.game_data_id = GameDataId::Emote;

    EmoteInfo e;
    e.id = id;
    e.Serialize(packet.dat.buf);

    packet.Serialize(packetData);
    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendDeathPacket(int id)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::DeathEvent);

    packet.dat.game_data_id = GameDataId::Emote;

    EmoteInfo e;
    e.id = id;
    e.Serialize(packet.dat.buf);

    packet.Serialize(packetData);
    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendRespawnPacket(int id)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::RespawnEvent);

    packet.dat.game_data_id = GameDataId::Emote;

    EmoteInfo e;
    e.id = id;
    e.Serialize(packet.dat.buf);

    packet.Serialize(packetData);
    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::sendNamePacket(int player_id)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.sender_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::SetUsername);

    packet.dat.game_data_id = GameDataId::Name;

    NameInfo n;
    n.player_id = player_id;
    n.name = m_nameMap.at(player_id);
    n.Serialize(packet.dat.buf);

    packet.Serialize(packetData);
    m_network->sendToAll(packetData, packetSize);
}

void ServerGame::receiveNamePacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset + sizeof(PacketHeader)]);
    NameInfo* n = reinterpret_cast<NameInfo*>(&dat->buf);

    m_nameMap[hdr->sender_id] = n->name;
    /*Player* player = reinterpret_cast<Player*>(EntitySpawner::instance()->GetEntity(ClassId::Player,
    hdr->sender_id));
    player->SetName(n->name);*/

    sendNamePacket(hdr->sender_id);
}