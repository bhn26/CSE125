#pragma once
//#define _WIN32
#ifdef _WIN32
#include <winsock2.h>
#include "ClientNetwork.h"
#include "../network/NetworkData.h"
#include "../network/DummyWorld.h"
#include "../network/GameData.h"
#include <Windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window;

class ClientGame
{
public:
#ifdef _WIN32
    ClientNetwork* network;
#endif
    GLFWwindow* window;

#ifdef _WIN32
    void sendActionPackets();

    void receiveInitPacket(int offset);

    // The data we want in network_data should have an offset if any
    void receiveSpawnPacket(int offset);
    void sendSpawnPacket();

    void receiveMovePacket(int offset);
    void sendMovePacket(int direction);

    void receiveVRotationPacket(int offset);
    void sendVRotationPacket(float v_rot, float h_rot); 

    char network_data[MAX_PACKET_SIZE];

    void update();
#endif

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

#ifdef _WIN32
    DummyWorld* world;
#endif

    static void Error_callback(int error, const char* description);
    void Setup_callbacks();
    void Setup_glew();
    void Setup_opengl_settings();
    void Print_versions();
};

