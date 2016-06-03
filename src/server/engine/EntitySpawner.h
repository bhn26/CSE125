#pragma once

// NOTE:  Only use by  EntitySpawner::instance()->"...methodcall..."
#include <map>
#include "Entity.h"
#include "Weapon.h"
#include "Powerup.h"
#include "BulletCollisionHandler.h"
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
    unsigned int oid_field;  // field
	~EntitySpawner();

	static EntitySpawner *instance();

	Player* spawnPlayer(int teamid, PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	Flag*  spawnFlag(PosInfo pos, btDiscreteDynamicsWorld* physicsWorld);

	// id of player spawning, team id of player spawning, dmg the bullet does, the type of weapon that shot the bullet, the collision handler for the bullet, the bullet physics body, the world
	Bullet* spawnBullet(int playerid, int teamid, int damage, WeaponType shooter, BulletCollisionHandler* handler, btRigidBody* bullet_body, btDiscreteDynamicsWorld* physicsWorld);

	void spawnCollectable(btDiscreteDynamicsWorld* curworld, WeaponType w_type);

	void spawnCollectable(btDiscreteDynamicsWorld* curworld, PowerupType p_type);

	void AddEntity(int cid, unsigned int oid, Entity* ent);

	Entity* GetEntity(int cid, unsigned int oid);

	void RemoveEntity(int cid, unsigned int oid);

	std::map<std::pair<int, unsigned int>, Entity* > * GetMap();

	static std::pair<int, int> getRandomLoc();

};