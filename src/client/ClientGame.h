#pragma once
//#define _WIN32
#ifdef _WIN32
#include <winsock2.h>
#include "ClientNetwork.h"
#include "../network/NetworkData.h"
#include "../network/GameData.h"
#include <Windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Player.h"
#include <memory>
#include <vector>

class Window;

class ClientGame
{
public:
#ifdef _WIN32
    ClientNetwork* network;
#endif
    GLFWwindow* window;

#ifdef _WIN32

    void receiveInitPacket(int offset);
	void sendInitPacket();

	void receiveJoinPacket(int offset);
	void sendJoinPacket(int team);

	// Tell the server this client is ready, this is like an ACK to prevent a race condition
	void sendReadyPacket();

	void receiveStartPacket(int offset);
	void sendStartPacket();

	void receiveReadyToSpawnPacket(int offset);

    // The data we want in network_data should have an offset if any
    void receiveSpawnPacket(int offset);

	void receiveRemovePacket(int offset);

    void receiveMovePacket(int offset);
    void sendMovePacket(int direction);

    void receiveRotationPacket(int offset);
    void sendRotationPacket(); 

	void sendJumpPacket();

	void receiveScorePacket(int offset);

	void receiveGameOverPacket(int offset);

	void sendShootPacket();


	bool hasStarted() { return game_started; };

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
	static int GetClientId() { return cg->client_id; }

	static std::vector<int> Team0() { return cg->team0; }
	static std::vector<int> Team1() { return cg->team1; }

	int TotalEggs() { return total_eggs; };
	int * GetScores() { return scores; };
	int GetClientTeam() { return client_team; };

private:
    ClientGame(void);
    ~ClientGame(void);

    double lastTime;
    int nbFrames;

    int client_id; // should know what client number we are so we can fill out packet headers
	int client_team;
	int client_skin;

	std::vector <int> team0;
	std::vector <int> team1;

	int total_eggs;
	int scores[2];
	int winner; // set on receipt of game over packet

	int tick = 0;

	bool game_started = false;
	bool iSpawned = false;

    static ClientGame* cg;

    static void Error_callback(int error, const char* description);
    void Setup_callbacks();
    void Setup_glew();
    void Setup_opengl_settings();
    void Print_versions();
};

