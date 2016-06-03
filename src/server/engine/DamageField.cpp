
#include "DamageField.h"
#include "Player.h"
#include "Entity.h"
#include "ObjectId.h"
#include "FireRateReset.h"

DamageField::DamageField(int ttl, int damage, btVector3* origin, btCollisionShape* fieldshape, int team_id, btDiscreteDynamicsWorld* curworld) : FieldObject(origin, fieldshape, team_id, curworld)
{
	this->fieldTtl = ttl;
	this->fieldDamage = damage;
}

DamageField::~DamageField()
{
	this->curWorld->removeCollisionObject(FieldGhostObject);
	delete FieldGhostObject->getCollisionShape();
	delete FieldGhostObject;
}

int DamageField::handleField()
{
	fieldTtl--;
	int numOverlap = FieldGhostObject->getNumOverlappingObjects();
	for (int i = 0; i < numOverlap; i++)
	{
		btRigidBody *pRigidBody = dynamic_cast<btRigidBody *>(FieldGhostObject->getOverlappingObject(i));
		if (pRigidBody && pRigidBody->getUserIndex() == ClassId::PLAYER)
		{
			Player * collidedPlayer = (Player *)pRigidBody->getUserPointer();
			if (collidedPlayer->GetTeamId() != team_id)
			{
				if (collidedPlayer->takeDamage(this->fieldDamage, FireRateReset::instance()->currentWorldTick))
				{
					//TODO Handle Player Death: send player death to client...  Maybe handle this on player
				}
			}
		}
	}
	
	if(fieldTtl > 0)
		return 0;
	else
		return 1;
}