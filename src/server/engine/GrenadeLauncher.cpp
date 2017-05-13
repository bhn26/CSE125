#include "GrenadeLauncher.h"
#include "ExplosiveBulletCollision.h"

GrenadeLauncher::GrenadeLauncher(btDiscreteDynamicsWorld* curworld) : Weapon(fireRate, damage, curworld)
{
	wt = WeaponType::GrenadeLauncher;
	nextFireTick = 0;
	fireRate = 100;
	damage = 1;
	currentAmmo = 12;
	maxAmmo = 12;
	gunSpeed = new btVector3(0, 15, 30);

}

GrenadeLauncher::~GrenadeLauncher() {}

int GrenadeLauncher::UseWeapon(btVector3* position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{
	if (this->fireFlag)
	{
		btCollisionShape* grenadeShape = new btSphereShape(btScalar(0.5));

		// Create Grenade physics object
		btVector3 globalPos = ((*rotation) * (btVector3(0, 5, 5))) + (*position);

		btDefaultMotionState*grenadeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), (globalPos)));
		btScalar mass = 1;
		btVector3 grenadeInertia(0, 0, 0);
		grenadeShape->calculateLocalInertia(mass, grenadeInertia);
		btRigidBody::btRigidBodyConstructionInfo grenadeRigidBodyCI(mass, grenadeMotionState, grenadeShape, grenadeInertia);
		btRigidBody* bRigidBody = new btRigidBody(grenadeRigidBodyCI);

		// set Grenade rotation
		btTransform currentTrans;
		bRigidBody->getMotionState()->getWorldTransform(currentTrans);
		currentTrans.setBasis((*rotation));
		bRigidBody->getMotionState()->setWorldTransform(currentTrans);
		bRigidBody->setCenterOfMassTransform(currentTrans);

		// set grenade velocity
		btVector3 newVelocity = (*rotation) * (*gunSpeed);

		bRigidBody->setLinearVelocity(newVelocity);
		bRigidBody->forceActivationState(DISABLE_DEACTIVATION);

		ExplosiveBulletCollision* handler = new ExplosiveBulletCollision(EXPLOSION_SIZE, EXPLOSION_DUR);
		Bullet* fireProjectile = EntitySpawner::Instance()->spawnBullet(playerid, teamid, this->damage, wt, handler, bRigidBody, curWorld);
		handler->SetBullet(fireProjectile);

		// Spawns bullet with this gun's damage, speed, and necessary ids into world
		this->fireFlag = 0;
		this->nextFireTick = FireRateReset::Instance()->currentWorldTick + fireRate;

		// add used weapon to "used" list in FireRateReset static object
		FireRateReset::Instance()->AddWeapon(this);
		currentAmmo--;
	}
	return currentAmmo;
}

//TODO reload weapon clip?
void GrenadeLauncher::ReloadWeapon()
{
}