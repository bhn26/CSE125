#pragma once

// NOTE:  Only use by  EntitySpawner::instance()->"...methodcall..."
#include <map>
#include "Entity.h"
#include "../../network/GameData.h"
//class Entity;
class Player;
class Flag;
class Bullet;
class Collectable;

class EntitySpawner
{
private:
	std::map<std::pair<int, unsigned int>, Entity* > entities;
	static EntitySpawner *spawnInstance;
	EntitySpawner();
	unsigned int oid_player;  // player
	unsigned int oid_flag;  // flag
	unsigned int oid_bullet;  // bullet
	unsigned int oid_collectable;  // collectable

public:

	~EntitySpawner();

	static EntitySpawner *instance();

	Player* spawnPlayer(int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	Flag*  spawnFlag(PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	Bullet* spawnBullet(int playerid, int teamid, int damage, const btVector3* pos, btVector3* velocity, btDiscreteDynamicsWorld* physicsWorld);

	Collectable* spawnCollectable(int objectid, PosInfo pos, btDiscreteDynamicsWorld* curworld);

	void AddEntity(int cid, unsigned int oid, Entity* ent);

	Entity* GetEntity(int cid, unsigned int oid);

	void RemoveEntity(int cid, unsigned int oid);

	std::map<std::pair<int, unsigned int>, Entity* > * GetMap();
};