#include "EntitySpawner.h"
#include "Entity.h"
#include "Bullet.h"
#include "Player.h"
#include "Flag.h"
#include "../ServerGame.h"

//Collectables
#include "Collectable.h"
#include "CollectableSpawner.h"
#include "SeedGun.h"

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
	oid_player = 0;
	oid_flag = 0;
	oid_bullet = 0;
	oid_collectable = 0;
}

EntitySpawner::~EntitySpawner(){}

Player* EntitySpawner::spawnPlayer(int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
{
	// Create player and add to Entity Map
	// note - need to create players with explicit id or else teams will depend on order they connected to the lobby
	Player* newPlayer = new Player(pos.id, pos.team_id, pos, physicsWorld);
	AddEntity(ClassId::PLAYER, pos.id, newPlayer);
	oid_player++;

	btQuaternion quat = newPlayer->GetEntityRotation();

	// Send Player Spawn packet
	btVector3 vec = newPlayer->GetEntityPosition();
	printf("Created player %d at (%f,%f,%f) on team %d\n", pos.id, vec.getX(), vec.getY(), vec.getZ(),pos.team_id );
	// Send spawn info to the clients
	PosInfo out;
	out.cid = ClassId::PLAYER;
	out.oid = newPlayer->GetObjectId();
	out.skin = pos.skin;
	out.team_id = pos.team_id;
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	out.rotw = quat.getW();
	out.rotx = quat.getX();
	out.roty = quat.getY();
	out.rotz = quat.getZ();
	
	ServerGame::instance()->sendSpawnPacket(out);
	return newPlayer;
}

Flag*  EntitySpawner::spawnFlag(PosInfo pos, btDiscreteDynamicsWorld* physicsWorld)
{
	// Create flag and add to Entity Map
	Flag* newFlag = new Flag(oid_flag, pos, physicsWorld);
	AddEntity(ClassId::FLAG, oid_flag, newFlag);
	oid_flag++;

	// Send Flag Spawn packet
	btVector3 vec = newFlag->GetEntityPosition();
	btQuaternion quat = newFlag->GetEntityRotation();
	printf("Created flag at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());

	PosInfo out;
	out.cid = ClassId::FLAG;
	out.oid = newFlag->GetObjectId();
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	out.rotw = quat.getW();
	out.rotx = quat.getX();
	out.roty = quat.getY();
	out.rotz = quat.getZ();
	ServerGame::instance()->sendSpawnPacket(out);
	return newFlag;
}

Bullet* EntitySpawner::spawnBullet(int playerid, int teamid, int damage, btVector3* pos, btVector3* velocity, btMatrix3x3* rotation, btDiscreteDynamicsWorld* physicsWorld)
{
	// Create Bullet and add to Entity Map
	Bullet* fireProjectile = new Bullet(oid_bullet, playerid, teamid, damage, pos, velocity, rotation, physicsWorld);
	AddEntity(ClassId::BULLET, oid_bullet, fireProjectile);
	oid_bullet++;

	// Send Bullet Spawn packet
	btVector3 vec = fireProjectile->GetEntityPosition();
	btQuaternion quat = fireProjectile->GetEntityRotation();
	//printf("Created Bullet at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());

	PosInfo out;
	out.cid = ClassId::BULLET;
	out.oid = fireProjectile->GetObjectId();
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	out.rotw = quat.getW();
	out.rotx = quat.getX();
	out.roty = quat.getY();
	out.rotz = quat.getZ();
	ServerGame::instance()->sendSpawnPacket(out);
	return fireProjectile;
}


Collectable* EntitySpawner::spawnCollectable(PosInfo pos, btDiscreteDynamicsWorld* curWorld, CollectableType col_type)
{
	Weapon* wp;
	switch (col_type)
		{
		case CollectableType::SEEDGUN:
		{
			wp = new SeedGun(curWorld);
			break;
		}
		default:
		{
				wp = nullptr;
				break;
		}
	}

	if (wp == nullptr)
		return;

	Collectable* ranCollectable = new Collectable(oid_collectable, pos, curWorld);
	AddEntity(ClassId::COLLECTABLE, oid_collectable, ranCollectable);
	oid_collectable++;

	// Send Collectable Spawn packet
	btVector3 vec = ranCollectable->GetEntityPosition();
	btQuaternion quat = ranCollectable->GetEntityRotation();
	printf("Created Collectable at (%f,%f,%f)\n", vec.getX(), vec.getY(), vec.getZ());

	PosInfo out;
	out.cid = ClassId::COLLECTABLE;
	out.oid = ranCollectable->GetObjectId();
	out.x = vec.getX();
	out.y = vec.getY();
	out.z = vec.getZ();
	out.rotw = quat.getW();
	out.rotx = quat.getX();
	out.roty = quat.getY();
	out.rotz = quat.getZ();
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
	//printf("? supposedly removed entity from map\n");
}

std::map<std::pair<int, unsigned int>, Entity* > *EntitySpawner::GetMap()
{
	return (&entities);
}

std::pair<int, int> EntitySpawner::getRandomLoc()
{
	std::pair<int, int> loc;
	loc.first = 0;
	loc.second = 0;
	while (loc.first == 0 && loc.second == 0)
	{
		if (rand() % 4 == 0)
		{
			loc.first = (rand() % WORLD_WIDTH + 1);
			loc.second = (rand() % WORLD_WIDTH + 1);
		}
		else if (rand() % 4 == 1)
		{
			loc.first = (rand() % WORLD_WIDTH + 1);
			loc.second = (-1 * rand() % WORLD_WIDTH + 1);
		}
		else if (rand() % 4 == 2)
		{
			loc.first = (-1 * rand() % WORLD_WIDTH + 1);
			loc.second = (rand() % WORLD_WIDTH + 1);
		}
		else if (rand() % 4 == 3)
		{
			loc.first = (-1 * rand() % WORLD_WIDTH + 1);
			loc.second = (-1 * rand() % WORLD_WIDTH + 1);
		}
	}
	return loc;
}

