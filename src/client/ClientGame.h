#pragma once
//#define _WIN32
#ifdef _WIN32
#include <winsock2.h>
#include "ClientNetwork.h"
#include "network/NetworkData.h"
#include "network/GameData.h"
#include <Windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <map>

#include <chrono>
#include <ratio>
#include <thread>

#include "Player.h"
#include "Graphics/Objects/Entity.h"
#include "Audio/SoundsHandler.h"

class Window;

namespace Controller
{
    enum class Buttons
    {
        A = 0,
        B = 1,
        X = 2,
        Y = 3,
        LBumper = 4,
        RBumper = 5,
        Back = 6,
        Start = 7,
        LAnalog = 8,
        RAnalog = 9,
        DPadUp = 10,
        DPadRight = 11,
        DPadDown = 12,
        DPadLeft = 13,
    };

    enum class Axes
    {
        LHorizontal = 0,
        LVertical = 1,
        RHorizontal = 4,
        RVertical = 3,
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
    void sendMovePacket(MoveType direction);

	void receiveTimeStampPacket(int offset);

    void receiveRotationPacket(int offset);
    void sendRotationPacket(); 

	void sendJumpPacket();

	void sendDancePacket();
	void receiveDancePacket(int offset);

	void receiveDeathPacket(int offset);

	void receiveRespawnPacket(int offset);

	void receiveScorePacket(int offset);

	void receiveGameOverPacket(int offset);

	void sendAttackPacket(AttackType t);
	void receiveAttackPacket(int offset);   // do distinct animation for peck and weapon attack later?

	void sendDiscardPacket();
	void receiveDiscardPacket(int offset);  // do animation for weapon discard later?

	void sendNamePacket();
	void receiveNamePacket(int offset);

	bool hasStarted() { return game_started; };

	void decScore(int team, int amount) { scores[team] -= amount; }
	void incScore(int team, int amount) { scores[team] += amount; }

    std::uint8_t m_networkData[g_maxPacketSize];

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
	int GetCountdown() { return countdown; };
	int GetClientTeam() { return client_team; };

	int GetWinner() { return winner; };

	std::string GetName(int id) { return name_map[id]; }
	void SetName(std::string name);

    ////////////////////////////////////////////////////////////////////////////////
    // Sounds
#ifdef PlaySound
#undef PlaySound
#endif
    int PlaySound(const std::string& soundName, SoundsHandler::SoundOptions options = SoundsHandler::SoundOptions());
    bool StopSound(int index);
    void PlayMenuSound();
    void StopMenuSound();

private:
    const static std::string EventQuit;
    const static std::string EventJump;
    const static std::string EventWeaponAttack;
	const static std::string EventPeckAttack;
	const static std::string EventDiscardWeapon;
    const static std::string EventStart;
    const static std::string EventMoveForward;
    const static std::string EventMoveBackward;
    const static std::string EventMoveLeft;
    const static std::string EventMoveRight;
    const static std::string EventScoreboard;
    const static std::string EventTauntDance;
    const static std::string EventTauntDeath;
    const static std::string EventTauntPeck;

    ClientGame(void);
    ~ClientGame(void);

    static void Error_callback(int error, const char* description);
    void LoadConfigs();
    void Setup_callbacks();
    void Setup_glew();
    void Setup_opengl_settings();
    void Print_versions();
    void PrintFrameRate();

    void ShowLoadingScreen();

    ////////////////////////////////////////////////////////////////////////////////
    // Controller handling
    void CheckController();
    void HandleTriggers(const float* axes);
    void HandleLeftAnalog(const float* axes);
    void HandleRightAnalog(const float* axes);
    void HandleButtonPress(const unsigned char* buttons);
    void HandleButtonEvent(const std::string& event, bool buttonDown = true);

    ////////////////////////////////////////////////////////////////////////////////
    // Instance Variables
    double lastTime;
    int nbFrames;

    int client_id; // should know what client number we are so we can fill out packet headers
	int client_team;
	int client_skin;

    bool start_sent;

	std::vector <int> team0;
	std::vector <int> team1;
	std::map<int, std::string> name_map;

	int total_eggs;
	std::chrono::time_point<std::chrono::steady_clock> start_time;
	int countdown; // Seconds
	int scores[2];
	int winner; // set on receipt of game over packet

	int tick = 0;

	bool game_started = false;
	bool iSpawned = false;

    SoundsHandler m_soundsHandler;
    int m_menuSound = -1;
};

