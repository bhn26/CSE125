#include "MineBulletHandler.h"
#include "BlastField.h"
#include "FieldHandler.h"
#include "network/GameData.h"
#include "Player.h"
#include "EntitySpawner.h"
#include "../ServerGame.h"

MineBulletHandler::MineBulletHandler(FieldObject* field)
{
	this->field = field;
}

bool MineBulletHandler::HandleBulletCollision(unsigned int world_tick, Entity* collidee)
{
	// don't delete if static object
	if(collidee == nullptr)
		return false;
	if(bullet == nullptr)
		return false;

	if (collidee->GetClassId() == ClassId::PLAYER)
	{
		//btVector3 temp = bullet->GetEntityPosition();
		//btVector3* position = new btVector3(temp.getX(), temp.getY(), temp.getZ());

		//btCollisionShape* expSphere = new btSphereShape(btScalar(explosion_size));
		//BlastField* explosionField = new BlastField(nonFriendly, magnitude, ttl, bullet->GetDamage(), position, expSphere, bullet->GetTeamId(), bullet->GetPhysicsWorld());
		// setting location of the field
		Player* p = (Player *) collidee;
		if(p->GetTeamId() != bullet->GetTeamId()){
			btVector3 temp = bullet->GetEntityPosition();
			btTransform currentTrans = field->GetFieldGhostObject()->getWorldTransform();
			currentTrans.setOrigin(temp);
			field->GetFieldGhostObject()->setWorldTransform(currentTrans);
			FieldHandler::instance()->AddField(field);

            FieldHandler::instance()->AddField(field);
            PosInfo p;
            btVector3 origin = field->GetFieldGhostObject()->getWorldTransform().getOrigin();
            p.cid = ClassId::FIELD;
            p.oid = EntitySpawner::instance()->oid_field++;
            field->oid = p.oid;
            p.sub_id = field->GetFieldType();
            p.x = origin.getX();
            p.y = origin.getY();
            p.z = origin.getZ();
            p.rad = 17;   // hack cuz demo soon

            ServerGame::instance()->sendSpawnPacket(p);
			return true;
		}
	}
	return false;
}  