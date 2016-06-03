#include "TeleportGun.h"
#include "EntitySpawner.h"
#include "PlayerModifierCollision.h"


TeleportGun::TeleportGun(btDiscreteDynamicsWorld* curworld) : Weapon(fireRate, damage, curworld)
{
	wt = WeaponType::TELEPORTGUN;
	nextFireTick = 0;
	fireRate = 600;
	damage = 1;
	currentAmmo = 2;
	maxAmmo = 2;
	gunSpeed = new btVector3(0, 8, 2000);
}

TeleportGun::~TeleportGun() {}

int TeleportGun::UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{
	if (this->fireFlag)
	{
		btCollisionShape* bulletShape = new btSphereShape(btScalar(1.5));

		// Create bullet physics object
		btVector3 globalPos = ((*rotation) * (btVector3(0, 1, 5))) + (*position);

		btDefaultMotionState*bulletMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), (globalPos)));
		btScalar mass = 1;
		btVector3 bulletInertia(0, 0, 0);
		bulletShape->calculateLocalInertia(mass, bulletInertia);
		btRigidBody::btRigidBodyConstructionInfo bulletRigidBodyCI(mass, bulletMotionState, bulletShape, bulletInertia);
		btRigidBody* bRigidBody = new btRigidBody(bulletRigidBodyCI);

		// set bullet rotation
		btTransform currentTrans;
		bRigidBody->getMotionState()->getWorldTransform(currentTrans);
		currentTrans.setBasis((*rotation));
		bRigidBody->getMotionState()->setWorldTransform(currentTrans);
		bRigidBody->setCenterOfMassTransform(currentTrans);

		// set bullet velocity
		btVector3 newVelocity = (*rotation) * (*this->gunSpeed);

		bRigidBody->setLinearVelocity(newVelocity);
		bRigidBody->forceActivationState(DISABLE_DEACTIVATION);

		// use the simple handler for the seedgun
		PlayerModifierCollision* handler = new PlayerModifierCollision((Player *) owner);

		// Spawns bullet with this gun's damage, speed, and necessary ids into world
		Bullet* fireProjectile = EntitySpawner::instance()->spawnBullet(playerid, teamid, this->damage, wt, handler, bRigidBody, curWorld);
		handler->SetBullet(fireProjectile);

		this->fireFlag = 0;
		this->nextFireTick = FireRateReset::instance()->currentWorldTick + fireRate;

		// add used weapon to "used" list in FireRateReset static object
		FireRateReset::instance()->AddWeapon(this);
		currentAmmo--;
	}
	return currentAmmo;
}
