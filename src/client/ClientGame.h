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

namespace Controller
{
    enum Buttons
    {
        A = 0,
        B = 1,
        X = 2,
        Y = 3,
        L_BUMPER = 4,
        R_BUMPER = 5,
        BACK = 6,
        START = 7,
        L_ANALOG = 8,
        R_ANALOG = 9,
        D_PAD_UP = 10,
        D_PAD_RIGHT = 11,
        D_PAD_DOWN = 12,
        D_PAD_LEFT = 13,
    };

    enum Axes
    {
        L_HORIZONTAL = 0,
        L_VERTICAL = 1,
        R_HORIZONTAL = 4,
        R_VERTICAL = 3,
    };
}

class ClientGame
{
    friend class Window;
	friend class CPlayState;
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

    static ClientGame* instance()
    {
        static ClientGame* instance = new ClientGame();
        return instance;
    }
	static int GetClientId() { return instance()->client_id; }

	static std::vector<int> Team0() { return instance()->team0; }
	static std::vector<int> Team1() { return instance()->team1; }

	int TotalEggs() { return total_eggs; };
	int * GetScores() { return scores; };
	int GetClientTeam() { return client_team; };

private:
    const static std::string EVENT_QUIT;
    const static std::string EVENT_JUMP;
    const static std::string EVENT_ATTACK;
    const static std::string EVENT_START;
    const static std::string EVENT_MOVE_FORWARD;
    const static std::string EVENT_MOVE_BACKWARD;
    const static std::string EVENT_MOVE_LEFT;
    const static std::string EVENT_MOVE_RIGHT;
    const static std::string EVENT_SCOREBOARD;
    const static std::string EVENT_TAUNT;

    ClientGame(void);
    ~ClientGame(void);

    double lastTime;
    int nbFrames;

    int client_id; // should know what client number we are so we can fill out packet headers
	int client_team;
	int client_skin;

    bool start_sent;

	std::vector <int> team0;
	std::vector <int> team1;

	int total_eggs;
	int scores[2];
	int winner; // set on receipt of game over packet

	int tick = 0;

	bool game_started = false;
	bool iSpawned = false;

    static void Error_callback(int error, const char* description);
    void Setup_callbacks();
    void Setup_glew();
    void Setup_opengl_settings();
    void Print_versions();
    void PrintFrameRate();

    ////////////////////////////////////////////////////////////////////////////////
    // Controller handling
    void CheckController();
    void HandleLeftAnalog(const float* axes);
    void HandleRightAnalog(const float* axes);
    void HandleButtonPress(const unsigned char* buttons);
    void HandleButtonEvent(const std::string& event, bool buttonDown = true);
};

