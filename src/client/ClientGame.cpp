#include "ClientGame.h"

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Graphics/Scene.h"
#include "network/GameData.h"
#include "network/NetworkData.h"
#include "TextRenderer.h"
#include "client/LoadState.h"
#include "client/PlayState.h"
#include "client/GameOverState.h"
#include "ConfigManager.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/ModelManager.h"

const std::string ClientGame::EVENT_QUIT = "Quit";
const std::string ClientGame::EVENT_JUMP = "Jump";
const std::string ClientGame::EVENT_WEAPON_ATTACK = "Weapon_Attack";
const std::string ClientGame::EVENT_PECK_ATTACK = "Peck_Attack";
const std::string ClientGame::EVENT_DISCARD_WEAPON = "Discard_Weapon";
const std::string ClientGame::EVENT_START = "Start";
const std::string ClientGame::EVENT_MOVE_FORWARD = "Move_Forward";
const std::string ClientGame::EVENT_MOVE_BACKWARD = "Move_Backward";
const std::string ClientGame::EVENT_MOVE_LEFT = "Move_Left";
const std::string ClientGame::EVENT_MOVE_RIGHT = "Move_Right";
const std::string ClientGame::EVENT_SCOREBOARD = "Scoreboard";
const std::string ClientGame::EVENT_TAUNT_DANCE = "Taunt_Dance";
const std::string ClientGame::EVENT_TAUNT_DEATH = "Taunt_Death";
const std::string ClientGame::EVENT_TAUNT_PECK = "Taunt_Peck";

ClientGame::ClientGame(void)
{
#ifdef _WIN32
    network = new ClientNetwork();

    start_sent = false;

	sendInitPacket();
	//sendStartPacket(); // temp - will add start button

	Initialize();
    
#endif

}


ClientGame::~ClientGame(void)
{
    Destroy();
}

#ifdef _WIN32

// Do we want to create a new world every time we get a new init packet
void ClientGame::receiveInitPacket(int offset)
{
    
    struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);

    // Find out what our client id is from the init packet of the server
    client_id = hdr->receiver_id;

	printf("set client id to %d\n", client_id);

	//Initialize();
}

void ClientGame::sendInitPacket() {
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	// no client id yet
	packet.hdr.receiver_id = SERVER_ID;
	packet.hdr.packet_type = INIT_CONNECTION;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveJoinPacket(int offset) {
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct PosInfo* pi = (struct PosInfo *) &(dat->buf);

	// set our team if it's for us
	if (pi->id == client_id) {
		client_team = pi->team_id;
		printf("setting client team to %d for player %d\n", client_team, client_id);
	}

	printf("receiveJoinPacket for player %d on team %d\n", pi->id, pi->team_id);
	int player = pi->id;
	int team = pi->team_id;
	
	if (team == 0) {
		team0.erase(std::remove(team0.begin(), team0.end(), player), team0.end()); // erase from both lists
		team1.erase(std::remove(team1.begin(), team1.end(), player), team1.end());

		team0.push_back(player);
	} else {
		team0.erase(std::remove(team0.begin(), team0.end(), player), team0.end());
		team1.erase(std::remove(team1.begin(), team1.end(), player), team1.end());

		team1.push_back(player);
	}
};

void ClientGame::sendJoinPacket(int team) {
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;

	packet.hdr.sender_id = client_id;
	packet.hdr.receiver_id = SERVER_ID;
	packet.hdr.packet_type = JOIN_TEAM;

	PosInfo pi;
	pi.id = client_id;
	pi.team_id = team;
	pi.serialize(packet.dat.buf);

	packet.serialize(packet_data);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
};


void ClientGame::sendReadyPacket()
{
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;

	packet.hdr.packet_type = READY_GAME;
	packet.hdr.sender_id = client_id;

    printf("sending a ready packet\n");

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveStartPacket(int offset) {
	struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);
    struct PacketData* dat = (struct PacketData *) &(network_data[offset + sizeof(PacketHeader)]);
    
	struct PosInfo* pi = (struct PosInfo *) &(dat->buf);
	printf("received start packet for %d players\n", pi->id);

	Window::m_pStateManager->ChangeState(CPlayState::GetInstance(Window::m_pStateManager)); // start game

	game_started = true;
	total_eggs = (team0.size() + team1.size()) * 2 + 1;
	sendReadyPacket();
}

void ClientGame::sendStartPacket() {
    if (start_sent)
        return;

	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.hdr.sender_id = client_id;
	packet.hdr.receiver_id = SERVER_ID;
	packet.hdr.packet_type = START_GAME;

	packet.serialize(packet_data);

    start_sent = true;
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

// sendIndSpawnPacket
void ClientGame::receiveReadyToSpawnPacket(int offset)
{
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.hdr.packet_type = IND_SPAWN_EVENT;
	packet.hdr.sender_id = client_id;
	packet.hdr.receiver_id = SERVER_ID;

	PosInfo pi;
	pi.id = client_id;
	pi.team_id = client_team;
	printf("send IndSpawn Packet for player %d on team %d\n", client_id, pi.team_id);
	pi.skin = rand() % 3;

	pi.serialize(packet.dat.buf);

	packet.serialize(packet_data);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveSpawnPacket(int offset)
{
    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* p = (struct PosInfo *) (dat->buf);

	// spawn the thing
	Scene::Instance()->AddEntity((*p));

	if (!iSpawned && p->oid == client_id)
	{
		iSpawned = true;
	}
}

void ClientGame::receiveRemovePacket(int offset)
{
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct RemInfo* r = (struct RemInfo *) &(dat->buf);

	Scene::Instance()->RemoveEntity(r->rem_cid, r->rem_oid);

	// if it's a flag, then a player should've removed it
	if (r->rem_cid == ClassId::FLAG)
	{
		// change the player's score based on that flag
		if (r->rec_cid == ClassId::PLAYER)
		{
			incScore(((Player *)(Scene::Instance()->GetEntity(ClassId::PLAYER, r->rec_oid).get()))->GetTeam(), 1);
		}
	}
}

void ClientGame::receiveMovePacket(int offset)
{

    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* pi = (struct PosInfo *) &(dat->buf);

	//if(pi->oid == 0)
		//printf("received move packet for obj id %d. Its coordinates are: %f, %f, %f\n", pi->oid, pi->x, pi->y, pi->z);

	Scene::Instance()->GetEntity(pi->cid, pi->oid)->MoveTo(pi->x, pi->y, pi->z);

	// check the jump animation
	if (pi->cid == ClassId::PLAYER)
	{
		Scene::Instance()->GetEntity(pi->cid, pi->oid)->SetScore(pi->num_eggs);
		if(pi->jump == 0)
			((Player *)(Scene::Instance()->GetEntity(pi->cid, pi->oid).get()))->Jump();
	}

}

// Need to know what direction to move in
void ClientGame::sendMovePacket(int direction)
{
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = MOVE_EVENT;
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    PosInfo pi;
    pi.direction = direction;
	glm::quat rot = Scene::Instance()->GetPlayer()->Orientation();
	pi.rotw = rot.w;
	pi.rotx = rot.x;
	pi.roty = rot.y;
	pi.rotz = rot.z;
    pi.serialize(packet.dat.buf);

    packet.serialize(packet_data);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveRotationPacket(int offset) {
    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* pi = (struct PosInfo *) &(dat->buf);

	//printf("received a rotation packet with: %f, %f, %f, %f\n", pi->rotw, pi->rotx, pi->roty, pi->rotz);

	//std::shared_ptr<Player> target = FindTarget(pi->id);
	//server don't tell us how we rotate
	if (pi->oid != client_id) {
		Scene::Instance()->GetEntity(pi->cid, pi->oid)->RotateTo(pi->rotw, pi->rotx, pi->roty, pi->rotz);
		glm::quat quat = Scene::Instance()->GetEntity(pi->cid, pi->oid)->Orientation();
	}
	
	// Rotate it if it's not a player
	if (pi->cid != ClassId::PLAYER)
	{
		Scene::Instance()->GetEntity(pi->cid, pi->oid)->RotateTo(pi->rotw, pi->rotx, pi->roty, pi->rotz);
	}



	// left/right rotation
	/*glm::mat4 newToWorld = target->GetToWorld() * glm::rotate(glm::mat4(1.0f), pi->v_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	target->SetToWorld(newToWorld);

	// up/down rotation
	float newAngle = target->GetCamAngle() + pi->h_rotation;
	const static float pi2 = glm::pi<float>() / 2;
	target->SetCamAngle((newAngle > pi2) ? pi2 : ((newAngle < -pi2) ? -pi2 : newAngle));*/

}

void ClientGame::sendRotationPacket() {
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = V_ROTATION_EVENT;
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

	// send the rotation of the main player
    PosInfo pi;
	glm::quat rot = Scene::Instance()->GetPlayer()->Orientation();
	pi.rotw = rot.w;
	pi.rotx = rot.x;
	pi.roty = rot.y;
	pi.rotz = rot.z;

	//pi.h_rotation = h_rot;
    pi.serialize(packet.dat.buf);

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendJumpPacket()
{
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = JUMP_EVENT;
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveScorePacket(int offset) {
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct ScoreInfo* s = (struct ScoreInfo *) &(dat->buf);

	printf("received a score packet t0: %d   t1: %d\n", s->t0_score, s->t1_score);

	scores[0] = s->t0_score;
	scores[1] = s->t1_score;
}

void ClientGame::receiveGameOverPacket(int offset) {
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct ScoreInfo* s = (struct ScoreInfo *) &(dat->buf);

	if (s->t0_score == s->t1_score) {
		winner = -1;
	}
	else if (s->t0_score > s->t1_score) { // t0 win
		winner = 0;
	}
	else { // t1 win
		winner = 1;
	}

	if (winner == -1)
		printf("Game was a tie!\n");
	else
		printf("Team %d won!\n", winner);
	// change state to game over screen
	Window::m_pStateManager->ChangeState(GOState::GetInstance(Window::m_pStateManager));
}

void ClientGame::receiveTimeStampPacket(int offset) 
{
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct MiscInfo* m = (struct MiscInfo *) &(dat->buf);

	printf("TIME ATM in SECONDS: %d\n", m->misc1);
}

void ClientGame::sendAttackPacket(AttackType t) {
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.hdr.packet_type = ATTACK_EVENT;
	packet.hdr.sender_id = client_id;
	packet.hdr.receiver_id = SERVER_ID;

	MiscInfo m;
	m.misc1 = t;
	m.misc3 = Scene::Instance()->GetPlayer()->GetCamAngle();
	m.serialize(packet.dat.buf);

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveAttackPacket(int offset)
{
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct EmoteInfo* e = (struct EmoteInfo *) &(dat->buf); // later parse this to miscinfo
	((Player *)(Scene::Instance()->GetEntity(ClassId::PLAYER, e->id).get()))->Attack();
}

void ClientGame::sendDiscardPacket()
{

	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.hdr.packet_type = DISCARD_EVENT;
	packet.hdr.sender_id = client_id;
	packet.hdr.receiver_id = SERVER_ID;

	packet.serialize(packet_data);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

//	NOTE: We're going to use the sender id as the guy that's dancing instead of changing
void ClientGame::receiveDancePacket(int offset)
{
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct EmoteInfo* e = (struct EmoteInfo *) &(dat->buf);
	((Player *)(Scene::Instance()->GetEntity(ClassId::PLAYER, e->id).get()))->Dance();
}

void ClientGame::sendDancePacket()
{

	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.hdr.packet_type = DANCE_EVENT;
	packet.hdr.sender_id = client_id;
	packet.hdr.receiver_id = SERVER_ID;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveDeathPacket(int offset)
{
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct EmoteInfo* e = (struct EmoteInfo *) &(dat->buf);
	((Player *)(Scene::Instance()->GetEntity(ClassId::PLAYER, e->id).get()))->Die();

	decScore(((Player *)(Scene::Instance()->GetEntity(ClassId::PLAYER, e->id).get()))->GetTeam(), 
		((Player *)(Scene::Instance()->GetEntity(ClassId::PLAYER, e->id).get()))->GetScore());
}

void ClientGame::receiveRespawnPacket(int offset) 
{
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct EmoteInfo* e = (struct EmoteInfo *) &(dat->buf);
	((Player *)(Scene::Instance()->GetEntity(ClassId::PLAYER, e->id).get()))->SetAlive(true);

	// update state
	if (e->id == client_id) {
		CPlayState::GetInstance(Window::m_pStateManager)->Respawn();
	}
}

void ClientGame::sendNamePacket() {
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.hdr.packet_type = SET_USERNAME;
	packet.hdr.sender_id = client_id;
	packet.hdr.receiver_id = SERVER_ID;

	NameInfo n;
	n.player_id = client_id;
	n.name = name_map.at(client_id);
	n.serialize(packet.dat.buf);

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveNamePacket(int offset) {
	struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);
	struct PacketData* dat = (struct PacketData *) &(network_data[offset + sizeof(PacketHeader)]);
	struct NameInfo* n = (struct NameInfo *) &(dat->buf);

	name_map[n->player_id] = n->name;
	//Player* player = (Player*)Scene::Instance()->GetEntity(ClassId::PLAYER,n->player_id).get();
	//player->SetName(n->name);
}

void ClientGame::update()
{
    Packet packet;
    int data_length = network->receivePackets(network_data);

    if (data_length <= 0) 
    {
        return;     //no data recieved
    }

    int i = 0;
    while (i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));

        switch (packet.hdr.packet_type) {

            case INIT_CONNECTION:

                // offset for this will be the packet header
                receiveInitPacket(i);
                break;

			case SET_USERNAME:
				receiveNamePacket(i);
				break;

			case JOIN_TEAM:
				receiveJoinPacket(i + sizeof(PacketHeader));
				break;

			case START_GAME:
				receiveStartPacket(i);
				break;

			case READY_TO_SPAWN_EVENT:
				receiveReadyToSpawnPacket(i);
				break;

            case SPAWN_EVENT:
                // You want to offset the packet header
                receiveSpawnPacket(i + sizeof(PacketHeader));
                break;

			case REMOVE_EVENT:
				receiveRemovePacket(i + sizeof(PacketHeader));
				break;

            case MOVE_EVENT:
				if(game_started) // the game needs to start for the client before this can happen
					receiveMovePacket(i + sizeof(PacketHeader));
                break;

			case TIME_EVENT:
				receiveTimeStampPacket(i + sizeof(PacketHeader));
				break;

			case DANCE_EVENT:
				receiveDancePacket(i + sizeof(PacketHeader));
				break;

			case DEATH_EVENT:
				receiveDeathPacket(i + sizeof(PacketHeader));
				break;

			case RESPAWN_EVENT:
				receiveRespawnPacket(i + sizeof(PacketHeader));
				break;

			case V_ROTATION_EVENT:
				receiveRotationPacket(i + sizeof(PacketHeader));
				break;

			case ATTACK_EVENT:
				receiveAttackPacket(i + sizeof(PacketHeader));
				break;

			case UPDATE_SCORE:
				receiveScorePacket(i + sizeof(PacketHeader));
				break;

			case GAME_OVER:
				receiveGameOverPacket(i + sizeof(PacketHeader));
				break;

            default:
                printf("error in packet types %d\n", packet.hdr.packet_type);
                break;
        }
        i += sizeof(Packet);
    }
}
#endif  // ifdef _WIN32

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

    // Initialize the shaders
    ShaderManager::Instance()->LoadShaders();

	// show loading screen
	ShowLoadingScreen();

    ModelManager::Instance()->LoadModels();
    // Initialize objects/pointers for rendering
    Window::Initialize_objects();

	TextRenderer::Initialize();
    Scene::Initialize();

	// go to login menu
	Window::m_pStateManager->ChangeState(CMenuState::GetInstance(Window::m_pStateManager));

    double lastTime = glfwGetTime();
    int nbFrames = 0;
	srand(time(NULL));
}

void ClientGame::ShowLoadingScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.28f, 0.65f, 0.89f, 1.0f); // reset color
	SpriteRenderer * sprite_renderer = new SpriteRenderer();
	Texture * bg = new Texture(GL_TEXTURE_2D, "assets/ui/loading/loading.png");
	int x = Texture::GetWindowCenter(bg->Width());
	int y = Window::height / 2 - bg->Height() / 2;

	sprite_renderer->DrawSprite(*bg, glm::vec2(x, y), glm::vec2(bg->Width(), bg->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	glfwSwapBuffers(window);

	delete sprite_renderer;
	delete bg;
}

void ClientGame::Destroy()
{
    Window::Clean_up();
    // Destroy the window
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
}


void ClientGame::GameLoop()
{
    while (!glfwWindowShouldClose(window))
    {
#ifdef _WIN32
        update();
#endif

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


void ClientGame::Error_callback(int error, const char* description)
{
    // Print error
    fputs(description, stderr);
}

void ClientGame::LoadConfigs()
{
    ConfigManager::instance()->LoadConfigs("eggs.cfg");
}

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

void ClientGame::Print_versions()
{
    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    //If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

// Prints once per 5 seconds
void ClientGame::PrintFrameRate()
{
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 5.0) // If last prinf() was more than 1 sec ago
    {
        printf("%f ms/frame\n", 5000.0/double(nbFrames));   // printf and reset timer
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
        return;

    // Get axes and buttons
    int axesCount, buttonCount;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

    // Check if we're starting the game
    if (!ClientGame::instance()->hasStarted())
    {
        if (buttons[Buttons::START])     // Start
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
void ClientGame::HandleTriggers(const float * axes)
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
void ClientGame::HandleRightAnalog(const float * axes)
{
    using namespace Controller;
    const float rotThreshold = 0.2f;
    if (abs(axes[Axes::R_HORIZONTAL]) > rotThreshold || abs(axes[Axes::R_VERTICAL]) > rotThreshold)
    {
        Scene::Instance()->GetPlayer()->ProcessViewMovement(abs(axes[Axes::R_HORIZONTAL]) > rotThreshold ? axes[Axes::R_HORIZONTAL] : 0.0f,
            abs(axes[Axes::R_VERTICAL]) > rotThreshold ? -axes[Axes::R_VERTICAL] : 0.0f);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Handles movement
void ClientGame::HandleLeftAnalog(const float * axes)
{
    using namespace Controller;
    const float threshold = 0.7f;
    int greatestAxis = abs(axes[Axes::L_HORIZONTAL]) > abs(axes[Axes::L_VERTICAL]) ? Axes::L_HORIZONTAL : Axes::L_VERTICAL;
    if (abs(axes[greatestAxis]) > threshold)
    {
        switch (greatestAxis)
        {
            case Axes::L_HORIZONTAL:     // Right
                if (axes[greatestAxis] > 0)
                    ClientGame::instance()->sendMovePacket(MOVE_RIGHT);
                else
                    ClientGame::instance()->sendMovePacket(MOVE_LEFT);
                break;
            case Axes::L_VERTICAL:     // DOWN
                if (axes[greatestAxis] > 0)
                    ClientGame::instance()->sendMovePacket(MOVE_BACKWARD);
                else
                    ClientGame::instance()->sendMovePacket(MOVE_FORWARD);
                break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Handles all button press. Currently does nothing
void ClientGame::HandleButtonPress(const unsigned char* buttons)
{
    using namespace Controller;
    if (buttons[Buttons::A])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_A"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_A"), false);
    if (buttons[Buttons::B])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_B"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_B"), false);
    if (buttons[Buttons::X])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_X"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_X"), false);
    if (buttons[Buttons::Y])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Y"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Y"), false);
    if (buttons[Buttons::BACK])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Back"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Back"), false);
    if (buttons[Buttons::START])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Start"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_Start"), false);
    if (buttons[Buttons::L_ANALOG])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Analog"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Analog"), false);
    if (buttons[Buttons::R_ANALOG])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Analog"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Analog"), false);
    if (buttons[Buttons::L_BUMPER])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Bumper"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_L_Bumper"), false);
    if (buttons[Buttons::R_BUMPER])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Bumper"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_R_Bumper"), false);
    if (buttons[Buttons::D_PAD_UP])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Up"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Up"), false);
    if (buttons[Buttons::D_PAD_RIGHT])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Right"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Right"), false);
    if (buttons[Buttons::D_PAD_DOWN])
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Down"));
    else
        HandleButtonEvent(ConfigManager::instance()->GetConfigValue("XBOX_D_Pad_Down"), false);
    if (buttons[Buttons::D_PAD_LEFT])
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
        if (event == EVENT_QUIT)
        {
            glfwSetWindowShouldClose(this->window, GL_TRUE);
        }
        else if (event == EVENT_WEAPON_ATTACK)
        {
            sendAttackPacket(AttackType::WEAPON_ATTACK);
        }
		else if (event == EVENT_PECK_ATTACK)
		{
			sendAttackPacket(AttackType::PECK);
		}
		else if (event == EVENT_DISCARD_WEAPON)
		{
			sendDiscardPacket(); // TODOCHANGETHIS
		}
        else if (event == EVENT_JUMP)
        {
            sendJumpPacket();
        }
        else if (event == EVENT_MOVE_FORWARD)
        {
            sendMovePacket(MOVE_FORWARD);
        }
        else if (event == EVENT_MOVE_BACKWARD)
        {
            sendMovePacket(MOVE_BACKWARD);
        }
        else if (event == EVENT_MOVE_LEFT)
        {
            sendMovePacket(MOVE_LEFT);
        }
        else if (event == EVENT_MOVE_RIGHT)
        {
            sendMovePacket(MOVE_RIGHT);
        }
        else if (event == EVENT_SCOREBOARD)
        {
            if (Window::m_pStateManager->GetActiveState() == CPlayState::GetInstance(Window::m_pStateManager))
            {
                CPlayState::GetInstance(Window::m_pStateManager)->show_scoreboard = true;
            }
        }
        else if (event == EVENT_TAUNT_DANCE)
        {
			printf("dance event triggered\n");
			sendDancePacket();
        }
        else if (event == EVENT_TAUNT_DEATH)
        {
            Scene::Instance()->GetPlayer()->Die();
        }
        else if (event == EVENT_TAUNT_PECK)
        {
            Scene::Instance()->GetPlayer()->Peck();
        }
    }
    else        // !buttonDown
    {
        if (event == EVENT_SCOREBOARD)
        {
            if (Window::m_pStateManager->GetActiveState() == CPlayState::GetInstance(Window::m_pStateManager))
            {
                CPlayState::GetInstance(Window::m_pStateManager)->show_scoreboard = false;
            }
        }
    }
}
