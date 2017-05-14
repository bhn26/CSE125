#include "RespawnHandler.h"

#include "EntitySpawner.h"
#include "server/ServerGame.h"

void RespawnHandler::KillPlayer(Player* player)
{
    m_respawnList.push_back(player);
}

// Goes through list of players that need respawning and respawns if needed
void RespawnHandler::RespawnPlayers(unsigned int current_tick)
{
    auto it = m_respawnList.begin();
    while (it != m_respawnList.end())
    {
        if ((*it)->GetDeathTime() + s_respawnTimeout == current_tick)
        {
            RespawnAPlayer((*it));
            it = m_respawnList.erase(it);
        }
        else
        {
            it++;
        }
    }
}

// Respawns a specific player
void RespawnHandler::RespawnAPlayer(Player* player)
{
    // reset hitpoints
    player->SetHitPoints(100);
    player->SetAlive(true);
    player->SetJumpSem();

    btTransform currentTrans;

    // Teleport Player to a random spot in the world
    std::pair<int, int> loc = EntitySpawner::GetRandomLoc();
    btVector3 ranPos = btVector3(loc.first, 90, loc.second);
    player->GetRigidBody()->getMotionState()->getWorldTransform(currentTrans);
    currentTrans.setOrigin(ranPos);
    player->GetRigidBody()->getMotionState()->setWorldTransform(currentTrans);
    player->GetRigidBody()->setCenterOfMassTransform(currentTrans);
    ServerGame::Instance()->SendRespawnPacket(player->GetObjectId());
}
