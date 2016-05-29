#pragma once
#include "Player.h"

class RespawnHandler
{
private:
	std::vector<Player*> respawnList;   // List of players that need respawning
	static const int RESPAWN_TIMEOUT = 200;      // players wait this number of ticks before they can respawn again

public:
	static RespawnHandler* instance()
	{
		static RespawnHandler* instance = new RespawnHandler();
		return instance;
	}

	// Adds a player to the respawn list, on which tick did this guy die?
	void KillPlayer(Player* p);

	// Goes through list of players that need respawning and respawns if needed, which tick are we respawning on
	void RespawnPlayers(unsigned int current_tick);

	// Respawns a specific player
	void RespawnAPlayer(Player* P);


};