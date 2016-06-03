#include "ExplosiveBulletCollision.h"
#include "DamageField.h"
#include "FieldHandler.h"
#include "../ServerGame.h"
#include "EntitySpawner.h"

// Does not consider what collided with it
bool ExplosiveBulletCollision::HandleBulletCollision(unsigned int world_tick, Entity* collidee) {
	if (bullet == nullptr)
		return false;
	
	btVector3 temp = bullet->GetEntityPosition();
	btVector3* position = new btVector3(temp.getX(), temp.getY(), temp.getZ());

	btCollisionShape* expSphere = new btSphereShape(btScalar(explosion_size));
	DamageField* explosionField = new DamageField(ttl, bullet->GetDamage(), position, expSphere, bullet->GetTeamId(), bullet->GetPhysicsWorld());

	//TODO: Add to damage field checker. Add this new field to the checker
	FieldHandler::instance()->AddField(explosionField);
    PosInfo p;
    btVector3 origin = explosionField->GetFieldGhostObject()->getWorldTransform().getOrigin();
    p.cid = ClassId::FIELD;
    p.oid = EntitySpawner::instance()->oid_field++;
    explosionField->oid = p.oid;
    p.sub_id = explosionField->GetFieldType();
    p.x = origin.getX();
    p.y = origin.getY();
    p.z = origin.getZ();
    p.rad = explosion_size;

    ServerGame::instance()->sendSpawnPacket(p);

	return true;
}