
#include "ClientGame.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "../Graphics/Scene.h"
#include "../network/NetworkData.h"
#include "TextRenderer.h"

#include "../client/PlayState.h"
//#define _WIN32

ClientGame* ClientGame::cg = nullptr;

ClientGame::ClientGame(void)
{
#ifdef _WIN32
    network = new ClientNetwork();

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
	sendReadyPacket();
}

void ClientGame::sendStartPacket() {
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.hdr.sender_id = client_id;
	packet.hdr.receiver_id = SERVER_ID;
	packet.hdr.packet_type = START_GAME;
	packet.hdr.sender_id = client_id;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveSpawnPacket(int offset)
{

    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* p = (struct PosInfo *) (dat->buf);

	// spawn the thing
	Scene::Instance()->AddEntity(p->cid, p->oid, p->x, p->y, p->z, p->rotw, p->rotx, p->roty, p->rotz);

	if (!iSpawned && p->oid == client_id)
	{
		iSpawned = true;
	}
}

void ClientGame::receiveRemovePacket(int offset)
{
	struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
	struct RemInfo* r = (struct RemInfo *) &(dat->buf);

	printf("received a remove packet for type %d object %d\n", r->rem_cid, r->rem_oid);

	Scene::Instance()->RemoveEntity(r->rem_cid, r->rem_oid);
}

void ClientGame::receiveMovePacket(int offset)
{

    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* pi = (struct PosInfo *) &(dat->buf);

	//if(pi->oid == 1)
		//printf("received move packet for obj id %d. Its coordinates are: %f, %f, %f\n", pi->oid, pi->x, pi->y, pi->z);

	Scene::Instance()->GetEntity(pi->cid, pi->oid)->MoveTo(pi->x, pi->y, pi->z);
	Scene::Instance()->GetEntity(pi->cid, pi->oid)->SetScore(pi->num_eggs);

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
	glm::quat rot = Scene::Instance()->GetPlayer()->GetOrientation();
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
		printf("rotation of player %d on client is %f, %f, %f, %f\n", pi->oid, quat.w, quat.x, quat.y, quat.z);
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
	glm::quat rot = Scene::Instance()->GetPlayer()->GetOrientation();
	pi.rotw = rot.w;
	pi.rotx = rot.x;
	pi.roty = rot.y;
	pi.rotz = rot.z;

	//printf("sending a rotation packet with: %f, %f, %f, %f\n", pi.rotw, pi.rotx, pi.roty, pi.rotz);
   // pi.v_rotation = v_rot;
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

			case JOIN_TEAM:
				receiveJoinPacket(i + sizeof(PacketHeader));
				break;

			case START_GAME:
				receiveStartPacket(i);
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

			case V_ROTATION_EVENT:
				receiveRotationPacket(i + sizeof(PacketHeader));
				break;

			case UPDATE_SCORE:
				receiveScorePacket(i + sizeof(PacketHeader));
				break;

            default:
                printf("error in packet types %d\n", packet.hdr.packet_type);
                break;
        }
        i += sizeof(Packet);
    }
}
#endif

void ClientGame::Initialize()
{
    // Create the GLFW window
    window = Window::Create_window(1024, 768);
    // Print OpenGL and GLSL versions
    Print_versions();
    // Setup callbacks
    Setup_callbacks();
    // Setup OpenGL settings, including lighting, materials, etc.
    Setup_opengl_settings();
    // Initialize objects/pointers for rendering
    Window::Initialize_objects();

	TextRenderer::Initialize();
    Scene::Initialize();

    double lastTime = glfwGetTime();
    int nbFrames = 0;
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
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 5.0) // If last prinf() was more than 1 sec ago
        {
            printf("%f ms/frame\n", 5000.0/double(nbFrames));   // printf and reset timer
            nbFrames = 0;
            lastTime += 5.0;
        }

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

void ClientGame::Setup_callbacks()
{
    // Set the error callback
    glfwSetErrorCallback(ClientGame::Error_callback);
    // Set the key callback
    glfwSetKeyCallback(this->window, Window::Key_callback);

    glfwSetCursorPosCallback(window, Window::Mouse_callback);
    glfwSetMouseButtonCallback(window, Window::Mouse_button_callback);
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
