#include "SeedGun.h"
#include "SimpleBulletCollision.h"

SeedGun::SeedGun(btDiscreteDynamicsWorld* curworld): Weapon(fireRate, damage, curworld)
{
	wt = WeaponType::SEEDGUN;
	nextFireTick = 0;
	fireRate = 5;
	damage = 15;
	currentAmmo = 20;
	maxAmmo = 12;
	gunSpeed = new btVector3(0, 5, 25);
}

SeedGun::~SeedGun(){}

int SeedGun::UseWeapon(btVector3* position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{
	if (this->fireFlag)
	{
		btCollisionShape* bulletShape = new btSphereShape(btScalar(0.5));

		// Create bullet physics object
		btVector3 globalPos = ((*rotation) * (btVector3(0, 1, 10))) + (*position);

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
		btVector3 newVelocity = (* rotation) * (* this->gunSpeed);

		bRigidBody->setLinearVelocity(newVelocity);
		bRigidBody->forceActivationState(DISABLE_DEACTIVATION);

		// use the simple handler for the seedgun
		SimpleBulletCollision* handler = new SimpleBulletCollision();

		// Spawns bullet with this gun's damage, speed, and necessary ids into world
		Bullet* fireProjectile = EntitySpawner::instance()->spawnBullet(playerid, teamid, this->damage, wt, handler, bRigidBody, curWorld);

		this->fireFlag = 0;
		this->nextFireTick = FireRateReset::instance()->currentWorldTick + fireRate;

		// add used weapon to "used" list in FireRateReset static object
		FireRateReset::instance()->AddWeapon(this);
		currentAmmo--;
	}
	return currentAmmo;
}
