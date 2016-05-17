#include "EntitySpawner.h"

EntitySpawner* EntitySpawner::spawnInstance = nullptr;

EntitySpawner* EntitySpawner::instance()
{
	if (!spawnInstance)
	{
		spawnInstance = new EntitySpawner();
	}
	return spawnInstance;
}

Player* EntitySpawner::spawnPlayer(int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
{
	Player* newPlayer = new Player(oid0, teamid, pos, physicsWorld);
	AddEntity(0, oid0, newPlayer);
	oid0++;
	return newPlayer;
}

Flag*  EntitySpawner::spawnFlag(PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
{
	Flag* newFlag = new Flag(oid1, pos, physicsWorld);
	AddEntity(1, oid1, newFlag);
	oid1++;
	return newFlag;
}

Bullet* EntitySpawner::spawnBullet(int playerid, int teamid, int damage, const btVector3* pos, btVector3* velocity, btDiscreteDynamicsWorld* physicsWorld)
{
	Bullet* fireProjectile = new Bullet(oid2, playerid, teamid, damage, pos, velocity, physicsWorld);
	AddEntity(2, oid2, fireProjectile);
	oid2++;
	return fireProjectile;
}

void EntitySpawner::AddEntity(int cid, unsigned int oid, Entity* ent)
{
	std::pair<int, unsigned int> p = std::pair<int, unsigned int>(cid, oid);
	//entities[p] = std::move(ent);
	entities[p] = ent;
}