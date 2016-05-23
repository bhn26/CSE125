
#include "Peck.h"
#include "DamageField.h"

#ifndef BULLET_PHYSICS
#define BULLET_PHYSICS
#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>
#endif

Peck::Peck(btDiscreteDynamicsWorld* curworld): Weapon(meleefireRate, meleeDamage, curworld)
{
	nextFireTick = 0;
}

Peck::~Peck(){}

void Peck::UseWeapon(const btVector3* position, btMatrix3x3* rotation, int playerid, int teamid)
{
	if (this->fireFlag)
	{
		// Spawns damage field for peck attack into the world
		btVector3 vec = btVector3(0, 0, -1);
		vec = ((vec) * (*rotation));
		vec = ((vec) + (*position));
		btVector3 * fieldPos = new btVector3(vec.getX(), vec.getY(), vec.getZ());
		//btCollisionShape* peckSphere = new btSphereShape(btVector3());

		//TODO!!!!!!!!__________________________________

		//DamageField* peckField = new DamageField(1, meleeDamage, fieldPos, );
		this->fireFlag = 0;
		this->nextFireTick = currentWorldTick + meleefireRate;

		// add used weapon to "used" list in FireRateReset static object
		std::shared_ptr<Weapon> weapon = std::shared_ptr<Weapon>(this);
		FireRateReset::instance()->AddWeapon(weapon);
	}
}