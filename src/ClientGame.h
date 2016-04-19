#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "network/client/ClientNetwork.h"
#include "network/NetworkData.h"
#include "network/DummyWorld.h"
#include "network/GameData.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window;

class ClientGame
{
public:
    ClientNetwork* network;
    GLFWwindow* window;

    void sendActionPackets();

    void receiveInitPacket(int offset);

    // The data we want in network_data should have an offset if any
    void receiveSpawnPacket(int offset);
    void sendSpawnPacket();

    void receiveMovePacket(int offset);
    void sendMovePacket(int direction);

    char network_data[MAX_PACKET_SIZE];

    void update();

    void Initialize();
    void Destroy();
    void GameLoop();

    static void instantiate() { 
        if (cg == NULL)
            cg = new ClientGame();
    }
    static ClientGame* instance() {return cg;}

private:
    ClientGame(void);
    ~ClientGame(void);

    double lastTime;
    int nbFrames;
    int client_id; // should know what client number we are so we can fill out packet headers

    static ClientGame* cg;

    DummyWorld* world;

    static void Error_callback(int error, const char* description);
    void Setup_callbacks();
    void Setup_glew();
    void Setup_opengl_settings();
    void Print_versions();
};

