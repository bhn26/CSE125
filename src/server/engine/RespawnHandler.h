#pragma once
#include "Basic/Singleton.h"
#include "Player.h"

class RespawnHandler : public Singleton<RespawnHandler>
{
    friend class Singleton<RespawnHandler>;

private:
    std::vector<Player*> m_respawnList; // List of players that need respawning

    // players wait this number of ticks before they can respawn again
    static const int s_respawnTimeout = 200;

public:
    // Adds a player to the respawn list, on which tick did this guy die?
    void KillPlayer(Player* p);

    // Goes through list of players that need respawning and respawns if needed, which tick are we
    // respawning on
    void RespawnPlayers(unsigned int current_tick);

    // Respawns a specific player
    void RespawnAPlayer(Player* P);
};