#pragma once
#include <stdlib.h>
#include "ServerNetwork.h"
#include "../network/NetworkData.h"
#include "engine/engine.h"
#include <btBulletDynamicsCommon.h>

#include <map>
#include <iostream>
#include <chrono>
#include <ratio>
#include <thread>

class ServerGame
{
public:
    static unsigned int NumClients() { return m_clientId; }
    void update();

    void receiveFromClients();

    // Want singleton for the world, if we receive an init packet from a new client
    // we want to send them the current world data, not reset the world
    // Assume 1 client for now
    void receiveInitPacket(int offset);
    void sendInitPacket();

    void receiveJoinPacket(int offset);
    void sendJoinPacket(int client);

    // Starting the game packets
    void receiveStartPacket(int offset);
    void sendStartPacket();

    void sendLoadPacket();

    void receiveIndSpawnPacket(int offset);
    void sendReadyToSpawnPacket();

    // The data we want in m_networkData should have an offset if any
    void sendSpawnPacket(
        PosInfo pi); // Spawn an object with position pi, pi holds obj type and obj id

    // Send what you want to remove, with the object's ids
    void sendRemovePacket(ClassId rem_cid, int rem_oid);
    // this one is used when another object is responsible for removing an object, like if a chicken
    // collects an egg, then the chicken
    // is responsible. The chicken's information would be the rec_cid and rec_oid
    void sendRemovePacket(ClassId rem_cid, int rem_oid, ClassId rec_cid, int rec_oid);

    void sendRemovePacket(ClassId rem_cid,
                          int rem_oid,
                          ClassId rec_cid,
                          int rec_oid,
                          CollectType c_type,
                          int subtype);

    // Returns the direction to be moved, if it can't move there, returns BAD_MOVE
    void receiveMovePacket(int offset);
    // what type is the object moving and what is the id of the object moving?
    void sendMovePacket(ClassId class_id, int obj_id);

    void receiveRotationPacket(int offset);
    void sendRotationPacket(ClassId class_id, int obj_id);

    void receiveJumpPacket(int offset);

    // not used
    void sendScorePacket();

    void sendGameOverPacket(int winner);

    void sendTimeStampPacket(int diff);

    void receiveDancePacket(int offset);
    void sendDancePacket(int id);

    void sendDeathPacket(int id);

    void sendRespawnPacket(int id);

    void sendAttackPacket(int id);
    void receiveAttackPacket(int offset); // do distinct animation for peck and weapon attack later?

    void sendDiscardPacket(int id);        // not sent until there's an animation for this
    void receiveDiscardPacket(int offset); // do animation for weapon discard later?

    void sendNamePacket(int player_id);
    void receiveNamePacket(int offset);

    static ServerGame* instance()
    {
        static ServerGame* instance = new ServerGame();
        return instance;
    }

    void IncScore(int team, int n) { m_scores[team] += n; };
    void DecScore(int team, int n)
    {
        m_scores[team] -= n;
    }

    int* GetScores() { return m_scores; }
    int GetTotalEggs() { return m_totalEggs; };
    int GetTeam(int player) const
    {
        auto it = m_teamMap.find(player);
        if ( it != m_teamMap.end())
        {
            return it->second;
        }
        return -1;
    };

private:
    ServerGame(void);
    ~ServerGame(void);

    // IDs for the clients connecting for table in ServerNetwork
    static unsigned int m_clientId;

    std::map<int, std::string> m_nameMap;
    std::map<int, int> m_teamMap; // <player, team>

    // variables for starting the game

    bool m_gameStarted = false;
    bool m_gameOver = false;
    bool m_eggsSpawned = false;
    unsigned int m_readyClients = 0; // # of clients ready for the game
    unsigned int m_spawnedClients = 0;

    std::chrono::time_point<std::chrono::steady_clock> m_startTime;

    unsigned int m_totalEggs = 0;

    Engine* m_engine;

    // The ServerNetwork object
    ServerNetwork* m_network;

    // data buffer
    std::uint8_t m_networkData[g_maxPacketSize];

    // SCORES
    int m_scores[2];
};