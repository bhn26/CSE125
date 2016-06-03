#include "ExplosiveBulletCollision.h"
#include "DamageField.h"
#include "FieldHandler.h"

bool ExplosiveBulletCollision::HandleBulletCollision(unsigned int world_tick) {
	if (bullet == nullptr)
		return false;
	
	btVector3 temp = bullet->GetEntityPosition();
	btVector3* position = new btVector3(temp.getX(), temp.getY(), temp.getZ());

	btTransform currentTrans;
	bullet->GetRigidBody()->getMotionState()->getWorldTransform(currentTrans);
	btMatrix3x3 currentOrientation = currentTrans.getBasis();

	btCollisionShape* expSphere = new btSphereShape(btScalar(explosion_size));
	DamageField* explosionField = new DamageField(ttl, bullet->GetDamage(), position, expSphere, bullet->GetTeamId(), bullet->GetPhysicsWorld());

	//TODO: Add to damage field checker. Add this new field to the checker
	FieldHandler::instance()->AddField(explosionField);

	return true;
}