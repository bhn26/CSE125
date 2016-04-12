#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "network/client/ClientNetwork.h"
#include "network/NetworkData.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window;

class ClientGame
{
public:
	ClientGame(void);
	~ClientGame(void);

	ClientNetwork* network;
    GLFWwindow* window;

	void sendActionPackets();

    char network_data[MAX_PACKET_SIZE];

    void update();

    void Initialize();
    void Destroy();
    void GameLoop();

private:
    double lastTime;
    int nbFrames;

    static void Error_callback(int error, const char* description);
    void Setup_callbacks();
    void Setup_glew();
    void Setup_opengl_settings();
    void Print_versions();
};

