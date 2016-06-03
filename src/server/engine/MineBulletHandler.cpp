#include "MineBulletHandler.h"
#include "BlastField.h"
#include "FieldHandler.h"
#include "network/GameData.h"
#include "Player.h"

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
			return true;
		}
	}
	return false;
}