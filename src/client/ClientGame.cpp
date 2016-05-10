
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
void ClientGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

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

void ClientGame::receiveStartPacket(int offset) {
	struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);
    struct PacketData* dat = (struct PacketData *) &(network_data[offset + sizeof(PacketHeader)]);
    
	struct PosInfo* pi = (struct PosInfo *) &(dat->buf);
	printf("received start packet for %d players\n", pi->id);

	Scene::Instance()->ClearPlayers();

	for (int i = 0; i < pi->id; i++) {
		printf("add player %d\n", i);
		Scene::Instance()->AddPlayer(i);
	}
}

void ClientGame::sendStartPacket() {
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	//packet.hdr.sender_id = ClientGame::GetClientId();
	packet.hdr.receiver_id = SERVER_ID;
	packet.hdr.packet_type = START_GAME;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveSpawnPacket(int offset)
{

    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* p = (struct PosInfo *) (dat->buf);


}

void ClientGame::sendSpawnPacket()
{
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = SPAWN_EVENT;
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveMovePacket(int offset)
{

    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* pi = (struct PosInfo *) &(dat->buf);

	std::shared_ptr<Player> target = FindTarget(pi->id);
	
	/* probably gonna switch this to coordinates later on */
	target->ProcessKeyboard((DIRECTION)pi->direction, 1); // move (rename method 
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
    pi.serialize(packet.dat.buf);

    packet.serialize(packet_data);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::receiveVRotationPacket(int offset) {
    struct PacketData *dat = (struct PacketData *) &(network_data[offset]);
    struct PosInfo* pi = (struct PosInfo *) &(dat->buf);

	std::shared_ptr<Player> target = FindTarget(pi->id);

	// left/right rotation
	glm::mat4 newToWorld = target->GetToWorld() * glm::rotate(glm::mat4(1.0f), pi->v_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	target->SetToWorld(newToWorld);

	// up/down rotation
	float newAngle = target->GetCamAngle() + pi->h_rotation;
	const static float pi2 = glm::pi<float>() / 2;
	target->SetCamAngle((newAngle > pi2) ? pi2 : ((newAngle < -pi2) ? -pi2 : newAngle));
}

void ClientGame::sendVRotationPacket(float v_rot, float h_rot) {
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = V_ROTATION_EVENT;
    packet.hdr.sender_id = client_id;
    packet.hdr.receiver_id = SERVER_ID;

    PosInfo pi;
    pi.v_rotation = v_rot;
	pi.h_rotation = h_rot;
    pi.serialize(packet.dat.buf);

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

std::shared_ptr<Player> ClientGame::FindTarget(int tid) {
	if (tid == client_id) {
		return Scene::Instance()->GetPlayer();
	}
	else {
		std::vector<std::shared_ptr<Player>> players = Scene::Instance()->GetPlayers();

		for (int i = 0; i < players.size(); i++) {
			int pid = players.at(i)->GetID();
			if (tid == pid) {
				return players.at(i);	
			}
		}
	}
	printf("ERROR - couldn't find target in players");
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

			case START_GAME:
				receiveStartPacket(i);
				break;

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                //sendActionPackets();

                break;

            case SPAWN_EVENT:

                // You want to offset the packet header
                receiveSpawnPacket(i + sizeof(PacketHeader));

                break;

            case MOVE_EVENT:

                receiveMovePacket(i + sizeof(PacketHeader));
                break;

			case V_ROTATION_EVENT:
				receiveVRotationPacket(i + sizeof(PacketHeader));
				break;

            default:
                printf("error in packet types\n");
                break;
        }
        i += sizeof(Packet);
    }
}
#endif

void ClientGame::Initialize()
{
    // Create the GLFW window
    window = Window::Create_window(640, 480);
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
    glClearColor(0.2f, 0.2f, 0.5f, 1.0f);

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
