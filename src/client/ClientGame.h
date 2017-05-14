#pragma once
//#define _WIN32
#ifdef _WIN32
#include <winsock2.h>
#include "ClientNetwork.h"
#include "network/NetworkData.h"
#include "network/GameData.h"
#include <Windows.h>
#endif

#include "Basic/Singleton.h"

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

class ClientGame : public Singleton<ClientGame>
{
    friend class Singleton<ClientGame>;
    friend class Window;
    friend class CPlayState;

public:
#ifdef _WIN32
    ClientNetwork* m_network;
#endif
    GLFWwindow* m_window;

#ifdef _WIN32

    void ReceiveInitPacket(int offset);
    void SendInitPacket();

    void ReceiveJoinPacket(int offset);
    void SendJoinPacket(int team);

    // TeLl the server this client is ready, this is like an ACK to prevent a race condition
    void SendReadyPacket();

    void ReceiveStartPacket(int offset);
    void SendStartPacket();

    void ReceiveReadyToSpawnPacket(int offset);

    // The data we want in network_data should have an offset if any
    void ReceiveSpawnPacket(int offset);

    void ReceiveRemovePacket(int offset);

    void ReceiveMovePacket(int offset);
    void SendMovePacket(MoveType direction);

    void ReceiveTimeStampPacket(int offset);

    void ReceiveRotationPacket(int offset);
    void SendRotationPacket();

    void SendJumpPacket();

    void SendDancePacket();
    void ReceiveDancePacket(int offset);

    void ReceiveDeathPacket(int offset);

    void ReceiveRespawnPacket(int offset);

    void ReceiveScorePacket(int offset);

    void ReceiveGameOverPacket(int offset);

    void SendAttackPacket(AttackType t);
    void ReceiveAttackPacket(int offset); // do distinct animation for peck and weapon attack later?

    void SendDiscardPacket();
    void ReceiveDiscardPacket(int offset); // do animation for weapon discard later?

    void SendNamePacket();
    void ReceiveNamePacket(int offset);

    bool HasStarted() { return m_gameStarted; };

    void DecScore(int team, int amount) { m_scores[team] -= amount; }
    void IncScore(int team, int amount) { m_scores[team] += amount; }

    std::uint8_t m_networkData[g_maxPacketSize];

    void Update();
#endif

    void Initialize();
    void Destroy();
    void GameLoop();

    static int GetClientId() { return Instance()->m_clientId; }

    static const std::vector<int>& Team0() { return Instance()->m_team0; }
    static const std::vector<int>& Team1() { return Instance()->m_team1; }

    int TotalEggs() { return m_totalEggs; };
    int* GetScores() { return m_scores; };
    int GetCountdown() { return m_countdown; };
    int GetClientTeam() { return m_clientTeam; };

    int GetWinner() { return m_winner; };

    std::string GetName(int id) { return m_nameMap[id]; }
    void SetName(std::string name);

////////////////////////////////////////////////////////////////////////////////
// Sounds
#ifdef PlaySound
#undef PlaySound
#endif
    int PlaySound(const std::string& soundName,
                  SoundsHandler::SoundOptions options = SoundsHandler::SoundOptions());
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
    double m_lastTime;
    int m_nbFrames;

    int m_clientId; // should know what client number we are so we can fill out packet headers
    int m_clientTeam;
    int m_clientSkin;

    bool m_startSent;

    std::vector<int> m_team0;
    std::vector<int> m_team1;
    std::map<int, std::string> m_nameMap;

    int m_totalEggs;
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    int m_countdown; // Seconds
    int m_scores[2];
    int m_winner; // set on receipt of game over packet

    int m_tick = 0;

    bool m_gameStarted = false;
    bool m_iSpawned = false;

    SoundsHandler m_soundsHandler;
    int m_menuSound = -1;
};
