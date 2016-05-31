
#include "DamageField.h"
#include "Player.h"
#include "Entity.h"
#include "ObjectId.h"
#include "FireRateReset.h"

DamageField::DamageField(int ttl, int damage, btVector3* origin, btCollisionShape* fieldshape, Entity* fieldowner, btDiscreteDynamicsWorld* curworld) : FieldObject(origin, fieldshape, fieldowner, curworld)
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
	printf("DamageField handleField was called \n");
	fieldTtl--;
	if (fieldTtl < 1)
	{
		int numOverlap = FieldGhostObject->getNumOverlappingObjects();
		printf("num of overlapping objects %d \n", numOverlap);
		for (int i = 0; i < numOverlap; i++)
		{
			btRigidBody *pRigidBody = dynamic_cast<btRigidBody *>(FieldGhostObject->getOverlappingObject(i));
			if (pRigidBody && pRigidBody->getUserIndex() == ClassId::PLAYER)
			{
				Player * collidedPlayer = (Player *)pRigidBody->getUserPointer();
				if (collidedPlayer->GetObjectId() != fieldOwner->GetObjectId())
				{
					if (collidedPlayer->takeDamage(this->fieldDamage, FireRateReset::instance()->currentWorldTick))
					{
						printf("Player is dead!");
						//TODO Handle Player Death: send player death to client...  Maybe handle this on player
						return 1;
					}
				}
			}
		}
	}
	return 0;
}