#include "EntitySpawner.h"
#include "Entity.h"
#include "Bullet.h"
#include "Player.h"
#include "Flag.h"
#include "Collectable.h"
#include "../ServerGame.h"

EntitySpawner* EntitySpawner::spawnInstance = nullptr;

EntitySpawner* EntitySpawner::instance()
{
	if (!spawnInstance)
	{
		spawnInstance = new EntitySpawner();
	}
	return spawnInstance;
}

EntitySpawner::EntitySpawner()
{
	oid0 = 0;
	oid1 = 0;
	oid2 = 0;
	oid3 = 0;
}

EntitySpawner::~EntitySpawner(){}

Player* EntitySpawner::spawnPlayer(int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
{
	// Create player and add to Entity Map
	Player* newPlayer = new Player(oid0, teamid, pos, physicsWorld);
	AddEntity(0, oid0, newPlayer);
	oid0++;

	// Send Player Spawn packet
	btVector3 vec = newPlayer->GetEntityPosition();
	printf("Created player at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());
	// Send spawn info to the clients
	PosInfo out;
	out.cid = ClassId::PLAYER;
	out.oid = newPlayer->GetId();
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	ServerGame::instance()->sendSpawnPacket(out);
	return newPlayer;
}

Flag*  EntitySpawner::spawnFlag(PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
{
	// Create flag and add to Entity Map
	Flag* newFlag = new Flag(oid1, pos, physicsWorld);
	AddEntity(1, oid1, newFlag);
	oid1++;

	// Send Flag Spawn packet
	btVector3 vec = newFlag->GetEntityPosition();
	printf("Created flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());

	PosInfo out;
	out.cid = ClassId::FLAG;
	out.oid = newFlag->GetObjectId();
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	ServerGame::instance()->sendSpawnPacket(out);
	return newFlag;
}

Bullet* EntitySpawner::spawnBullet(int playerid, int teamid, int damage, const btVector3* pos, btVector3* velocity, btDiscreteDynamicsWorld* physicsWorld)
{
	// Create Bullet and add to Entity Map
	Bullet* fireProjectile = new Bullet(oid2, playerid, teamid, damage, pos, velocity, physicsWorld);
	AddEntity(2, oid2, fireProjectile);
	oid2++;

	// Send Flag Spawn packet
	btVector3 vec = fireProjectile->GetEntityPosition();
	printf("Created Bullet at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());

	PosInfo out;
	out.cid = ClassId::BULLET;
	out.oid = fireProjectile->GetObjectId();
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	ServerGame::instance()->sendSpawnPacket(out);
	return fireProjectile;
}


Collectable* EntitySpawner::spawnCollectable(int objectid, PosInfo pos, btDiscreteDynamicsWorld* curworld)
{
	Collectable* ranCollectable = new Collectable(oid3, pos, curworld);
	AddEntity(3, oid3, ranCollectable);
	oid3++;

	// Send Flag Spawn packet
	btVector3 vec = ranCollectable->GetEntityPosition();
	printf("Created Collectable at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());

	PosInfo out;
	out.cid = ClassId::COLLECTABLE;
	out.oid = ranCollectable->GetObjectId();
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	ServerGame::instance()->sendSpawnPacket(out);
	return ranCollectable;
}


void EntitySpawner::AddEntity(int cid, unsigned int oid, Entity* ent)
{
	std::pair<int, unsigned int> p = std::pair<int, unsigned int>(cid, oid);
	//entities[p] = std::move(ent);
	entities[p] = ent;
}

Entity* EntitySpawner::GetEntity(int cid, unsigned int oid)
{
	std::pair<int, unsigned int> key = std::pair<int, unsigned int>(cid, oid);
	return entities[key];
}

void EntitySpawner::RemoveEntity(int cid, unsigned int oid)
{
	std::map<std::pair<int, unsigned int>, Entity*>::iterator it;
	std::pair<int, unsigned int> key = std::pair<int, unsigned int>(cid, oid);
	it = this->entities.find(key);
	entities.erase(it);
}

std::map<std::pair<int, unsigned int>, Entity* > *EntitySpawner::GetMap()
{
	return (&entities);
}

