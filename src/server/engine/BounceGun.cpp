#include "BounceGun.h"
#include "BounceBulletCollision.h"

BounceGun::BounceGun(btDiscreteDynamicsWorld* curworld) : Weapon(fireRate, damage, curworld)
{
	wt = WeaponType::BounceGun;
	nextFireTick = 0;
	fireRate = 10;
	damage = 14;
	currentAmmo = 200;
	maxAmmo = 200;
	gunSpeed = new btVector3(0, 8, 40);
}

BounceGun::~BounceGun() {}

int BounceGun::UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{
	if (this->fireFlag)
	{
		btCollisionShape* bulletShape = new btSphereShape(btScalar(0.5));

		// Create bullet physics object
		btVector3 globalPos = ((*rotation) * (btVector3(0, 2, 14))) + (*position);

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
		btVector3* initial_speed = new btVector3(newVelocity.getX(), newVelocity.getY(), newVelocity.getZ());
		BounceBulletCollision* handler = new BounceBulletCollision(NUM_BOUNCES, initial_speed);

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