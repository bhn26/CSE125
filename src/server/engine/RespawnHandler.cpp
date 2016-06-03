#include "RespawnHandler.h"
#include "EntitySpawner.h"
#include "server\ServerGame.h"

void RespawnHandler::KillPlayer(Player* p)
{
	respawnList.push_back(p);
}

// Goes through list of players that need respawning and respawns if needed
void RespawnHandler::RespawnPlayers(unsigned int current_tick)
{
	auto it = respawnList.begin();
	while (it != respawnList.end())
	{
		if ((*it)->GetDeathTime() + RESPAWN_TIMEOUT == current_tick)
		{
			RespawnAPlayer((*it));
			it = respawnList.erase(it);
		}
		else
		{
			it++;
		}

	}
}

// Respawns a specific player
void RespawnHandler::RespawnAPlayer(Player* p)
{
	// reset hitpoints
	p->SetHitPoints(100);
	p->SetAlive(true);
	p->SetJumpSem();

	btTransform currentTrans;

	// Teleport Player to a random spot in the world
	std::pair<int, int> loc = EntitySpawner::getRandomLoc();
	btVector3 ranPos = btVector3(loc.first, 90, loc.second);
	p->GetRigidBody()->getMotionState()->getWorldTransform(currentTrans);
	currentTrans.setOrigin(ranPos);
	p->GetRigidBody()->getMotionState()->setWorldTransform(currentTrans);
	p->GetRigidBody()->setCenterOfMassTransform(currentTrans);
	ServerGame::instance()->sendRespawnPacket(p->GetObjectId());
}
