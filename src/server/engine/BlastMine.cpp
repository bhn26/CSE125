#include "BlastMine.h"
#include "EntitySpawner.h"
#include "MineBulletHandler.h"
#include "BlastField.h"

BlastMine::BlastMine(btDiscreteDynamicsWorld* curworld) : Weapon(fireRate, damage, curworld)
{
	wt = WeaponType::BlastMine;
	nextFireTick = 0;
	fireRate = 50;
	damage = 6;
	currentAmmo = 4;
	maxAmmo = 4;
	gunSpeed = new btVector3(0, 0, 0);
}

BlastMine::~BlastMine() {}

int BlastMine::UseWeapon(btVector3 * position, btMatrix3x3* rotation, int playerid, int teamid, Entity* owner)
{

	if (this->fireFlag)
	{
		btCollisionShape* bulletShape = new btCylinderShape(btVector3(2, 1, 2));

		// Create bullet physics object
		btVector3 globalPos = ((*rotation) * (btVector3(0, 1, 9))) + (*position);

		btDefaultMotionState*bulletMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), (globalPos)));
		btScalar mass = 1;
		btVector3 bulletInertia(0, 0, 0);
		bulletShape->calculateLocalInertia(mass, bulletInertia);
		btRigidBody::btRigidBodyConstructionInfo bulletRigidBodyCI(mass, bulletMotionState, bulletShape, bulletInertia);
		btRigidBody* bRigidBody = new btRigidBody(bulletRigidBodyCI);

		bRigidBody->setLinearFactor(btVector3(0,0,0));

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


		btCollisionShape* expSphere = new btSphereShape(btScalar(explosion_size));
		btVector3* vec = new btVector3(0,0,0); // field origin determined later by the handler
		BlastField* field = new BlastField(nonFriendly, stun, magnitude, ttl, damage, vec, expSphere, teamid, curWorld);

		// use the simple handler for the seedgun
		MineBulletHandler* handler = new MineBulletHandler(field);

		// Spawns bullet with this gun's damage, speed, and necessary ids into world
		Bullet* fireProjectile = EntitySpawner::Instance()->spawnBullet(playerid, teamid, this->damage, wt, handler, bRigidBody, curWorld);
		handler->SetBullet(fireProjectile);

		this->fireFlag = 0;
		this->nextFireTick = FireRateReset::Instance()->currentWorldTick + fireRate;

		// add used weapon to "used" list in FireRateReset static object
		FireRateReset::Instance()->AddWeapon(this);
		currentAmmo--;
	}
	return currentAmmo;
}