
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

void Peck::UseWeapon(btVector3* position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{
	printf("Player uses Peck!!!!!!!!!!!\n");
	printf("FireFlag: %d\n", this->fireFlag);
	if (this->fireFlag)
	{
		// Spawns damage field for peck attack into the world
		btVector3 vec = btVector3(0, 0, 1);
		vec = ((vec) * (*rotation));
		vec = ((vec) + (*position));
		btVector3 * fieldPos = new btVector3(vec.getX(), vec.getY(), vec.getZ());
		btCollisionShape* peckSphere = new btSphereShape(btScalar(.5));
		DamageField* peckField = new DamageField(1, meleeDamage, fieldPos, peckSphere, owner, curWorld);

		//TODO: Add to damage field checker. Add this new field to the checker
		FieldHandler::instance()->AddField(peckField);

		this->fireFlag = 0;
		this->nextFireTick = FireRateReset::instance()->currentWorldTick + meleefireRate;

		// add used weapon to "used" list in FireRateReset static object
		FireRateReset::instance()->AddWeapon(this);
	}
}

void Peck::ReloadWeapon() {}

