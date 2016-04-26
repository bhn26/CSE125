
#include "ClientGame.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>

#include "Window.h"
#include "Graphics/Scene.h"
//#define _WIN32

ClientGame* ClientGame::cg = nullptr;

ClientGame::ClientGame(void)
{
#ifdef _WIN32
    network = new ClientNetwork();

    // send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.hdr.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
#endif

    Initialize();
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
    printf("creating new client world\n");
    world = new DummyWorld();
    
    struct PacketHeader* hdr = (struct PacketHeader *) &(network_data[offset]);

    // Find out what our client id is from the init packet of the server
    client_id = hdr->receiver_id;
}

void ClientGame::receiveSpawnPacket(int offset)
{
    struct PosInfo* pi = (struct PosInfo *) &(network_data[offset]);
    world->spawnDummy(pi->x, pi->y);
    //printf("------------------------------------------------------\n");
    //printf("offset = %d\n", offset);
    printf("client spawned a dummy at (%d,%d)\n", pi->x, pi->y);
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
    struct PosInfo* pi = (struct PosInfo *) &(network_data[offset]);
    world->moveDummy(pi->direction);
    struct PosInfo dpi = world->getDummyPos();
    printf("Dummy moved to (%d,%d)\n", dpi.x, dpi.y);
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

    packet.pi.direction = direction;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
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
