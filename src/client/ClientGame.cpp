#include "ClientGame.h"

#include <cstdio>

#include <chrono>
#include <ratio>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Graphics/Scene.h"
#include "network/GameData.h"
#include "network/NetworkData.h"
#include "TextRenderer.h"
#include "client/PlayState.h"
#include "client/LobbyState.h"
#include "client/GameOverState.h"
#include "ConfigManager.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/ModelManager.h"
#include "Audio/SoundBufferManager.h"

const std::string ClientGame::EventQuit = "Quit";
const std::string ClientGame::EventJump = "Jump";
const std::string ClientGame::EventWeaponAttack = "Weapon_Attack";
const std::string ClientGame::EventPeckAttack = "Peck_Attack";
const std::string ClientGame::EventDiscardWeapon = "Discard_Weapon";
const std::string ClientGame::EventStart = "Start";
const std::string ClientGame::EventMoveForward = "Move_Forward";
const std::string ClientGame::EventMoveBackward = "Move_Backward";
const std::string ClientGame::EventMoveLeft = "Move_Left";
const std::string ClientGame::EventMoveRight = "Move_Right";
const std::string ClientGame::EventScoreboard = "Scoreboard";
const std::string ClientGame::EventTauntDance = "Taunt_Dance";
const std::string ClientGame::EventTauntDeath = "Taunt_Death";
const std::string ClientGame::EventTauntPeck = "Taunt_Peck";

////////////////////////////////////////////////////////////////////////////////
int ClientGame::PlaySound(const std::string& soundName, SoundsHandler::SoundOptions options)
{
    std::shared_ptr<sf::SoundBuffer> soundBuffer = SoundBufferManager::GetSoundBuffer(soundName);
    if (soundBuffer)
    {
        return m_soundsHandler.Play(*soundBuffer, options);
    }
    return -1;
}

bool ClientGame::StopSound(int index)
{
    return m_soundsHandler.StopSound(index);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::PlayMenuSound()
{
    std::shared_ptr<sf::SoundBuffer> soundBuffer = SoundBufferManager::GetSoundBuffer("Menu");
    if (soundBuffer)
    {
        // loop and set to origin
        SoundsHandler::SoundOptions options;
        options._loops = true;
        options._isRelativeToListener = true;
        m_menuSound = m_soundsHandler.Play(*soundBuffer, options);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::StopMenuSound()
{
    if (m_menuSound != -1)
    {
        m_soundsHandler.StopSound(m_menuSound);
        m_menuSound = -1;
    }
}

////////////////////////////////////////////////////////////////////////////////
ClientGame::ClientGame(void) : m_soundsHandler()
{
#ifdef _WIN32
    network = new ClientNetwork();

    start_sent = false;

    sendInitPacket();
    // sendStartPacket(); // temp - will add start button

    Initialize();

#endif
}

////////////////////////////////////////////////////////////////////////////////
ClientGame::~ClientGame(void)
{
    Destroy();
}

#ifdef _WIN32

////////////////////////////////////////////////////////////////////////////////
// Do we want to create a new world every time we get a new init packet
void ClientGame::receiveInitPacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&(m_networkData[offset]));

    // Find out what our client id is from the init packet of the server
    client_id = hdr->receiver_id;

    printf("set client id to %d\n", client_id);

    // Initialize();
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendInitPacket()
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    // no client id yet
    packet.hdr.receiver_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::InitConnection);

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveJoinPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&(m_networkData[offset]));
    PosInfo* pi = reinterpret_cast<PosInfo*>(&(dat->buf));

    // set our team if it's for us
    if (pi->id == client_id)
    {
        client_team = pi->team_id;
        printf("setting client team to %d for player %d\n", client_team, client_id);
    }

    printf("receiveJoinPacket for player %d on team %d\n", pi->id, pi->team_id);
    int player = pi->id;
    int team = pi->team_id;

    if (team == 0)
    {
        team0.erase(std::remove(team0.begin(), team0.end(), player),
                    team0.end()); // erase from both lists
        team1.erase(std::remove(team1.begin(), team1.end(), player), team1.end());

        team0.push_back(player);
    }
    else
    {
        team0.erase(std::remove(team0.begin(), team0.end(), player), team0.end());
        team1.erase(std::remove(team1.begin(), team1.end(), player), team1.end());

        team1.push_back(player);
    }
};

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendJoinPacket(int team)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;

    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::JoinTeam);

    PosInfo pi;
    pi.id = client_id;
    pi.team_id = team;
    pi.Serialize(packet.dat.buf);

    packet.Serialize(packetData);
    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
};

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendReadyPacket()
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;

    packet.hdr.packet_type = static_cast<int>(PacketTypes::ReadyGame);
    packet.hdr.sender_id = client_id;

    printf("sending a ready packet\n");

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveStartPacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&(m_networkData[offset]));
    PacketData* dat =
        reinterpret_cast<PacketData*>(&(m_networkData[offset + sizeof(PacketHeader)]));

    PosInfo* pi = reinterpret_cast<PosInfo*>(&(dat->buf));
    printf("received start packet for %d players\n", pi->id);

    Window::m_pStateManager->ChangeState(
        CPlayState::GetInstance(Window::m_pStateManager)); // start game

    game_started = true;
    total_eggs = (team0.size() + team1.size()) * 2 + 1;
    sendReadyPacket();
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendStartPacket()
{
    if (start_sent)
        return;

    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::StartGame);

    packet.Serialize(packetData);

    start_sent = true;
    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
// sendIndSpawnPacket
void ClientGame::receiveReadyToSpawnPacket(int offset)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::IndSpawnEvent);
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    PosInfo pi;
    pi.id = client_id;
    pi.team_id = client_team;
    printf("send IndSpawn Packet for player %d on team %d\n", client_id, pi.team_id);
    pi.skin = rand() % 3;

    pi.Serialize(packet.dat.buf);

    packet.Serialize(packetData);
    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveSpawnPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    PosInfo* p = reinterpret_cast<PosInfo*>(dat->buf);

    // spawn the thing
    Scene::Instance()->AddEntity((*p));

    if (!iSpawned && p->oid == client_id)
    {
        iSpawned = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::SetName(std::string name)
{
    if (name == "Enter your name" || name == "")
    {
        std::string def;
        def += "Player ";
        def += std::to_string(client_id);
        name_map[client_id] = def;
    }
    else
        name_map[client_id] = name;
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveRemovePacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    RemInfo* r = reinterpret_cast<RemInfo*>(&dat->buf);

    Scene::Instance()->RemoveEntity(r->rem_cid, r->rem_oid);

    // if it's a flag, then a player should've removed it
    if (r->rem_cid == ClassId::Flag)
    {
        // change the player's score based on that flag
        if (r->rec_cid == ClassId::Player)
        {
            incScore(((Player*)(Scene::Instance()->GetEntity(ClassId::Player, r->rec_oid).get()))
                         ->GetTeam(),
                     1);
        }
        glm::vec3 position =
            ((Player*)(Scene::Instance()->GetEntity(ClassId::Player, r->rec_oid).get()))
                ->Position();
        SoundsHandler::SoundOptions options(
            position.x, position.y, position.z); // Play at own position
        PlaySound("Collect_Egg", options);
    }
    if (r->rem_cid == ClassId::Collectable
        && static_cast<CollectType>(r->sub_id) == CollectType::Weapon) // If Weapon/collectable
    {
        // Tell the client you got a weapon!
        if (client_id == r->rec_oid)
        {
            if (Scene::Instance()->GetPlayer()->GetWeapon() == -1)
                Scene::Instance()->GetPlayer()->SetWeapon(r->sub_id2);
        }
        // make sound regardless of who it is
        glm::vec3 position =
            ((Player*)(Scene::Instance()->GetEntity(ClassId::Player, r->rec_oid).get()))
                ->Position();
        SoundsHandler::SoundOptions options(
            position.x, position.y, position.z); // Play at own position
        PlaySound("Collect_Weapon", options);
    }
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveMovePacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    PosInfo* pi = reinterpret_cast<PosInfo*>(&dat->buf);

    // if(pi->oid == 0)
    // printf("received move packet for obj id %d. Its coordinates are: %f, %f, %f\n", pi->oid,
    // pi->x, pi->y, pi->z);

    Scene::Instance()->GetEntity(pi->cid, pi->oid)->MoveTo(pi->x, pi->y, pi->z);

    // check the jump animation
    if (pi->cid == ClassId::Player)
    {
        Scene::Instance()->GetEntity(pi->cid, pi->oid)->SetScore(pi->num_eggs);
        if (pi->jump == 0 || pi->jump == 1)
            ((Player*)(Scene::Instance()->GetEntity(pi->cid, pi->oid).get()))->Jump();
    }

    // update hp
    ((Player*)(Scene::Instance()->GetEntity(pi->cid, pi->oid).get()))->SetHealth(pi->hp);
}

////////////////////////////////////////////////////////////////////////////////
// Need to know what direction to move in
void ClientGame::sendMovePacket(MoveType direction)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::MoveEvent);
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    PosInfo pi;
    pi.direction = static_cast<int>(direction);
    glm::quat rot = Scene::Instance()->GetPlayer()->Orientation();
    pi.rotw = rot.w;
    pi.rotx = rot.x;
    pi.roty = rot.y;
    pi.rotz = rot.z;
    pi.Serialize(packet.dat.buf);

    packet.Serialize(packetData);
    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveRotationPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    PosInfo* pi = reinterpret_cast<PosInfo*>(&dat->buf);

    // printf("received a rotation packet with: %f, %f, %f, %f\n", pi->rotw, pi->rotx, pi->roty,
    // pi->rotz);

    // std::shared_ptr<Player> target = FindTarget(pi->id);
    // server don't tell us how we rotate
    if (pi->oid != client_id)
    {
        Scene::Instance()
            ->GetEntity(pi->cid, pi->oid)
            ->RotateTo(pi->rotw, pi->rotx, pi->roty, pi->rotz);
        glm::quat quat = Scene::Instance()->GetEntity(pi->cid, pi->oid)->Orientation();
    }

    // Rotate it if it's not a player
    if (pi->cid != ClassId::Player)
    {
        Scene::Instance()
            ->GetEntity(pi->cid, pi->oid)
            ->RotateTo(pi->rotw, pi->rotx, pi->roty, pi->rotz);
    }

    // left/right rotation
    /*glm::mat4 newToWorld = target->GetToWorld() * glm::rotate(glm::mat4(1.0f), pi->v_rotation,
    glm::vec3(0.0f, 1.0f, 0.0f));
    target->SetToWorld(newToWorld);

    // up/down rotation
    float newAngle = target->GetCamAngle() + pi->h_rotation;
    const static float pi2 = glm::pi<float>() / 2;
    target->SetCamAngle((newAngle > pi2) ? pi2 : ((newAngle < -pi2) ? -pi2 : newAngle));*/
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendRotationPacket()
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::VRotationEvent);
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    // send the rotation of the main player
    PosInfo pi;
    glm::quat rot = Scene::Instance()->GetPlayer()->Orientation();
    pi.rotw = rot.w;
    pi.rotx = rot.x;
    pi.roty = rot.y;
    pi.rotz = rot.z;

    // pi.h_rotation = h_rot;
    pi.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendJumpPacket()
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::JumpEvent);
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveScorePacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    ScoreInfo* s = reinterpret_cast<ScoreInfo*>(&dat->buf);

    printf("received a score packet t0: %d   t1: %d\n", s->t0_score, s->t1_score);

    scores[0] = s->t0_score;
    scores[1] = s->t1_score;
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveGameOverPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    ScoreInfo* s = reinterpret_cast<ScoreInfo*>(&dat->buf);

    if (s->t0_score == s->t1_score)
    {
        winner = -1;
    }
    else if (s->t0_score > s->t1_score)
    { // t0 win
        winner = 0;
    }
    else
    { // t1 win
        winner = 1;
    }

    if (winner == -1)
        printf("Game was a tie!\n");
    else
        printf("Team %d won!\n", winner);
    // change state to game over screen
    Window::m_pStateManager->ChangeState(GOState::GetInstance(Window::m_pStateManager));
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveTimeStampPacket(int offset)
{
    PacketData* data = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    MiscInfo* m = reinterpret_cast<MiscInfo*>(&data->buf);
    if (!countdown)
    {
        start_time = std::chrono::steady_clock::now();
        countdown = 300 - m->misc1;
    }

    if (countdown != m->misc1)
    {
        countdown = 300 - m->misc1;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendAttackPacket(AttackType t)
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::AttackEvent);
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    MiscInfo m;
    m.misc1 = static_cast<int>(t);
    m.misc3 = Scene::Instance()->GetPlayer()->GetCamAngle();
    m.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveAttackPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    EmoteInfo* e = reinterpret_cast<EmoteInfo*>(&dat->buf); // later parse this to miscinfo
    auto player =
        reinterpret_cast<Player*>(Scene::Instance()->GetEntity(ClassId::Player, e->id).get());
    player->Attack();
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendDiscardPacket()
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::DiscardEvent);
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveDiscardPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&(m_networkData[offset]));
    MiscInfo* m = reinterpret_cast<MiscInfo*>(&dat->buf);
    if (m->misc1 == client_id)
    {
        Scene::Instance()->GetPlayer()->SetWeapon(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//	NOTE: We're going to use the sender id as the guy that's dancing instead of changing
void ClientGame::receiveDancePacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    EmoteInfo* e = reinterpret_cast<EmoteInfo*>(&dat->buf);
    auto player =
        reinterpret_cast<Player*>(Scene::Instance()->GetEntity(ClassId::Player, e->id).get());
    player->Dance();
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendDancePacket()
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::DanceEvent);
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveDeathPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset]);
    EmoteInfo* e = reinterpret_cast<EmoteInfo*>(&dat->buf);

    // model
    auto player =
        reinterpret_cast<Player*>(Scene::Instance()->GetEntity(ClassId::Player, e->id).get());
    player->Die();

    // score
    decScore(player->GetTeam(), player->GetScore());

    // death overlay
    if (e->id == client_id)
    {
        CPlayState::GetInstance(Window::m_pStateManager)->Die();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveRespawnPacket(int offset)
{
    PacketData* dat = reinterpret_cast<PacketData*>(m_networkData[offset]);
    EmoteInfo* e = reinterpret_cast<EmoteInfo*>(&dat->buf);
    auto player =
        reinterpret_cast<Player*>(Scene::Instance()->GetEntity(ClassId::Player, e->id).get());
    player->SetAlive(true);

    // update state
    if (e->id == client_id)
    {
        CPlayState::GetInstance(Window::m_pStateManager)->Respawn();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::sendNamePacket()
{
    const unsigned int packetSize = sizeof(Packet);
    std::uint8_t packetData[packetSize];

    Packet packet;
    packet.hdr.packet_type = static_cast<int>(PacketTypes::SetUsername);
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    NameInfo n;
    n.player_id = client_id;
    n.name = name_map.at(client_id);
    n.Serialize(packet.dat.buf);

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);

    packet.Serialize(packetData);

    NetworkServices::sendMessage(network->ConnectSocket, packetData, packetSize);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::receiveNamePacket(int offset)
{
    PacketHeader* hdr = reinterpret_cast<PacketHeader*>(&m_networkData[offset]);
    PacketData* dat = reinterpret_cast<PacketData*>(&m_networkData[offset + sizeof(PacketHeader)]);
    NameInfo* n = reinterpret_cast<NameInfo*>(&dat->buf);

    name_map[n->player_id] = n->name;
    // Player* player =
    // reinterpret_cast<Player*>(Scene::Instance()->GetEntity(ClassId::Player,n->player_id).get());
    // player->SetName(n->name);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::update()
{
    Packet packet;
    int data_length = network->receivePackets(m_networkData);

    if (data_length <= 0)
    {
        return; // no data recieved
    }

    unsigned int i = 0;
    while (i < (unsigned int)data_length)
    {
        packet.Deserialize(&m_networkData[i]);

        switch (static_cast<PacketTypes>(packet.hdr.packet_type))
        {
        case PacketTypes::InitConnection:
            // offset for this will be the packet header
            receiveInitPacket(i);
            break;

        case PacketTypes::SetUsername:
            receiveNamePacket(i);
            break;

        case PacketTypes::JoinTeam:
            receiveJoinPacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::StartGame:
            receiveStartPacket(i);
            break;

        case PacketTypes::ServerLoading:
            LobbyState::GetInstance(Window::m_pStateManager)->ServerLoading();
            break;

        case PacketTypes::ReadyToSpawnEvent:
            receiveReadyToSpawnPacket(i);
            break;

        case PacketTypes::SpawnEvent:
            // You want to offset the packet header
            receiveSpawnPacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::RemoveEvent:
            receiveRemovePacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::DiscardEvent:
            receiveDiscardPacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::MoveEvent:
            if (game_started) // the game needs to start for the client before this can happen
                receiveMovePacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::TimeEvent:
            receiveTimeStampPacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::DanceEvent:
            receiveDancePacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::DeathEvent:
            receiveDeathPacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::RespawnEvent:
            receiveRespawnPacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::VRotationEvent:
            receiveRotationPacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::AttackEvent:
            receiveAttackPacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::UpdateScore:
            receiveScorePacket(i + sizeof(PacketHeader));
            break;

        case PacketTypes::GameOver:
            receiveGameOverPacket(i + sizeof(PacketHeader));
            break;

        default:
            printf("error in packet types %d\n", packet.hdr.packet_type);
            break;
        }
        i += sizeof(Packet);
    }
}
#endif // ifdef _WIN32

////////////////////////////////////////////////////////////////////////////////
void ClientGame::Initialize()
{
    // Create the GLFW window
    window = Window::Create_window(1366, 768);

    // Print OpenGL and GLSL versions
    Print_versions();
    // Setup callbacks
    Setup_callbacks();
    // Setup OpenGL settings, including lighting, materials, etc.
    Setup_opengl_settings();

    // Initialize the shaders. Must be loaded before Loading screen to show the screen.
    ShaderManager::Instance()->LoadShaders();

    // show loading screen
    ShowLoadingScreen();

    ModelManager::Instance()->LoadModels();
    SoundBufferManager::Instance()->LoadSoundBuffers();
    // Initialize objects/pointers for rendering
    Window::Initialize_objects();

    TextRenderer::Initialize();
    Scene::Initialize();

    // go to login menu
    Window::m_pStateManager->ChangeState(CMenuState::GetInstance(Window::m_pStateManager));
    sf::Listener::setUpVector(0.0f, 1.0f, 0.0f); // Initialize the up vector
    for (int i = 0; i < SoundsHandler::MAX_SOUNDS; i++)
    {
        m_soundsHandler.SetMinDistance(i, ConfigManager::GetAsFloat("Sounds_Min_Distance"));
        m_soundsHandler.SetAttenuation(i, ConfigManager::GetAsFloat("Sounds_Attenuation"));
    }
    PlayMenuSound();

    double lastTime = glfwGetTime();
    int nbFrames = 0;
    srand(static_cast<unsigned int>(time(NULL)));
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::ShowLoadingScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.28f, 0.65f, 0.89f, 1.0f); // reset color
    SpriteRenderer* sprite_renderer = new SpriteRenderer();
    Texture* bg = new Texture(GL_TEXTURE_2D, "assets/ui/loading/loading.png");
    int x = Texture::GetWindowCenter(bg->Width());
    int y = Window::height / 2 - bg->Height() / 2;

    sprite_renderer->DrawSprite(*bg,
                                glm::vec2(x, y),
                                glm::vec2(bg->Width(), bg->Height()),
                                0.0f,
                                glm::vec3(1.0f, 1.0f, 1.0f));

    glfwSwapBuffers(window);

    delete sprite_renderer;
    delete bg;
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::Destroy()
{
    Window::Clean_up();
    // Destroy the window
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::GameLoop()
{
    while (!glfwWindowShouldClose(window))
    {
#ifdef _WIN32
        update();
#endif
        auto curr_time = std::chrono::steady_clock::now();

        auto fp_stamp = curr_time - start_time;

        int diff = fp_stamp.count();

        diff = diff / 1000;

        if ((300 - diff) == (countdown - 1))
            countdown = 300 - diff;

        // Measure speed
        PrintFrameRate();

        CheckController();

        // Main render display callback. Rendering of objects is done here.
        Window::Display_callback(window);
        // Idle callback. Updating objects, etc. can be done here.
        Window::Idle_callback();

        if (++tick % 15 == 0 && iSpawned)
        {
            ClientGame::instance()->sendRotationPacket();
            tick = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::Error_callback(int error, const char* description)
{
    // Print error
    fputs(description, stderr);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::LoadConfigs()
{
    ConfigManager::instance()->LoadConfigs("eggs.cfg");
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::Setup_callbacks()
{
    // Set the error callback
    glfwSetErrorCallback(ClientGame::Error_callback);
    // Set the key callback
    glfwSetKeyCallback(this->window, Window::Key_callback);

    glfwSetCursorPosCallback(window, Window::Mouse_callback);
    glfwSetMouseButtonCallback(window, Window::Mouse_button_callback);
    glfwSetCharCallback(window, Window::Char_callback);
    // Set the window resize callback
    glfwSetWindowSizeCallback(window, Window::Resize_callback);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::Setup_glew()
{
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::Setup_opengl_settings()
{
    // Setup GLEW
    Setup_glew();
    // Enable depth buffering
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon
    // You can also use the paramter of GL_LINE instead of GL_FILL to see wireframes
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Disable backface culling to render both sides of polygons
    glDisable(GL_CULL_FACE);
    // Set clear color
    glClearColor(0.28f, 0.65f, 0.89f, 1.0f);

    // Font Rendering
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

////////////////////////////////////////////////////////////////////////////////
void ClientGame::Print_versions()
{
    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

// If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::printf("Supported GLSL version is %s.\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Prints once per 5 seconds
void ClientGame::PrintFrameRate()
{
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 5.0) // If last prinf() was more than 1 sec ago
    {
        printf("%f ms/frame\n", 5000.0 / double(nbFrames)); // printf and reset timer
        nbFrames = 0;
        lastTime += 5.0;
    }
}

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// Checks the controller input on Microsoft PC-joystick driver
// axis[0] = (left) right
// axis[1] = (left) down
// axis[2] = (< 0) Left trigger, (> 0) Right trigger
// axis[3] = (right) down
// axis[4] = (right) right
// button[0-3] = a, b, x, y
// button[4-5] = LB, RB
// button[6-7] = Back, Start
// button[8-9] = left/right analog
// button[10-13] = d-pad up, right, down, left
void ClientGame::CheckController()
{
    using namespace Controller;
    if (!glfwJoystickPresent(GLFW_JOYSTICK_1))
    {
        return;
    }

    // Get axes and buttons
    int axesCount, buttonCount;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

    // Check if we're starting the game
    if (!ClientGame::instance()->hasStarted())
    {
        if (buttons[static_cast<int>(Buttons::Start)]) // Start
        {
            printf("client will send start game\n");
            printf("sending start packet\n");
            ClientGame::instance()->sendStartPacket();
        }
        return;
    }
    else
    {
        // Handle analog movement
        HandleLeftAnalog(axes);
        HandleRightAnalog(axes);
        HandleButtonPress(buttons);
    }
}

////////////////////////////////////////////////////////////////////////////////
// axis[2] = (< 0) Left trigger, (> 0) Right trigger
void ClientGame::HandleTriggers(const float* axes)
{
    static const float threshold = 0.5f;
    if (axes[2] < -threshold)
    {
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Trigger"));
    }
    if (axes[2] > threshold)
    {
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Trigger"));
    }
}

////////////////////////////////////////////////////////////////////////////////
// Handles View changing
void ClientGame::HandleRightAnalog(const float* axes)
{
    using namespace Controller;
    const float rotThreshold = 0.2f;
    if (abs(axes[static_cast<int>(Axes::RHorizontal)]) > rotThreshold
        || abs(axes[static_cast<int>(Axes::RVertical)]) > rotThreshold)
    {
        Scene::Instance()->GetPlayer()->ProcessViewMovement(
            abs(axes[static_cast<int>(Axes::RHorizontal)]) > rotThreshold
                ? axes[static_cast<int>(Axes::RHorizontal)]
                : 0.0f,
            abs(axes[static_cast<int>(Axes::RVertical)]) > rotThreshold
                ? -axes[static_cast<int>(Axes::RVertical)]
                : 0.0f);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Handles movement
void ClientGame::HandleLeftAnalog(const float* axes)
{
    using namespace Controller;
    const float threshold = 0.7f;
    int greatestAxis = abs(axes[static_cast<int>(Axes::LHorizontal)])
                               > abs(axes[static_cast<int>(Axes::LVertical)])
                           ? static_cast<int>(Axes::LHorizontal)
                           : static_cast<int>(Axes::LVertical);
    if (abs(axes[greatestAxis]) > threshold)
    {
        switch (greatestAxis)
        {
        case static_cast<int>(Axes::LHorizontal): // Right
            if (axes[greatestAxis] > 0)
            {
                ClientGame::instance()->sendMovePacket(MoveType::Right);
            }
            else
            {
                ClientGame::instance()->sendMovePacket(MoveType::Left);
            }
            break;
        case static_cast<int>(Axes::LVertical): // DOWN
            if (axes[greatestAxis] > 0)
            {
                ClientGame::instance()->sendMovePacket(MoveType::Backward);
            }
            else
            {
                ClientGame::instance()->sendMovePacket(MoveType::Forward);
            }

            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Handles all button press. Currently does nothing
void ClientGame::HandleButtonPress(const unsigned char* buttons)
{
    using namespace Controller;
    if (buttons[static_cast<int>(Buttons::A)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_A"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_A"), false);
    if (buttons[static_cast<int>(Buttons::B)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_B"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_B"), false);
    if (buttons[static_cast<int>(Buttons::X)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_X"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_X"), false);
    if (buttons[static_cast<int>(Buttons::Y)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Y"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Y"), false);
    if (buttons[static_cast<int>(Buttons::Back)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Back"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Back"), false);
    if (buttons[static_cast<int>(Buttons::Start)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Start"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Start"), false);
    if (buttons[static_cast<int>(Buttons::LAnalog)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Analog"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Analog"), false);
    if (buttons[static_cast<int>(Buttons::RAnalog)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Analog"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Analog"), false);
    if (buttons[static_cast<int>(Buttons::LBumper)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Bumper"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Bumper"), false);
    if (buttons[static_cast<int>(Buttons::RBumper)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Bumper"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Bumper"), false);
    if (buttons[static_cast<int>(Buttons::DPadUp)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Up"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Up"), false);
    if (buttons[static_cast<int>(Buttons::DPadRight)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Right"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Right"), false);
    if (buttons[static_cast<int>(Buttons::DPadDown)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Down"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Down"), false);
    if (buttons[static_cast<int>(Buttons::DPadLeft)])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Left"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Left"), false);
}

void ClientGame::HandleButtonEvent(const std::string& event, bool buttonDown)
{
    if (!event.size())
        return;
    if (buttonDown)
    {
        if (event == EventQuit)
        {
            glfwSetWindowShouldClose(this->window, GL_TRUE);
        }
        else if (event == EventWeaponAttack)
        {
            sendAttackPacket(AttackType::Weapon);
        }
        else if (event == EventPeckAttack)
        {
            sendAttackPacket(AttackType::Peck);
        }
        else if (event == EventDiscardWeapon)
        {
            sendDiscardPacket(); // TODOCHANGETHIS
        }
        else if (event == EventJump)
        {
            sendJumpPacket();
        }
        else if (event == EventMoveForward)
        {
            sendMovePacket(MoveType::Forward);
        }
        else if (event == EventMoveBackward)
        {
            sendMovePacket(MoveType::Backward);
        }
        else if (event == EventMoveLeft)
        {
            sendMovePacket(MoveType::Left);
        }
        else if (event == EventMoveRight)
        {
            sendMovePacket(MoveType::Right);
        }
        else if (event == EventScoreboard)
        {
            if (Window::m_pStateManager->GetActiveState()
                == CPlayState::GetInstance(Window::m_pStateManager))
            {
                CPlayState::GetInstance(Window::m_pStateManager)->show_scoreboard = true;
            }
        }
        else if (event == EventTauntDance)
        {
            printf("dance event triggered\n");
            sendDancePacket();
        }
        else if (event == EventTauntDeath)
        {
            Scene::Instance()->GetPlayer()->TauntDie();
        }
        else if (event == EventTauntPeck)
        {
            Scene::Instance()->GetPlayer()->Peck();
        }
    }
    else // !buttonDown
    {
        if (event == EventScoreboard)
        {
            if (Window::m_pStateManager->GetActiveState()
                == CPlayState::GetInstance(Window::m_pStateManager))
            {
                CPlayState::GetInstance(Window::m_pStateManager)->show_scoreboard = false;
            }
        }
    }
}
