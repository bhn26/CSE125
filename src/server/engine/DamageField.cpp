
#include "DamageField.h"
#include "Player.h"
#include "Entity.h"
#include "ObjectId.h"

DamageField::DamageField(int ttl, int damage, btVector3* origin, btCollisionShape* fieldshape, Entity* fieldowner, btDiscreteDynamicsWorld* curworld) : FieldObject(origin, fieldshape, fieldowner, curworld, ttl)
{
	this->fieldDamage = damage;
}

DamageField::~DamageField()
{
	this->curWorld->removeCollisionObject(FieldGhostObject);
	delete FieldGhostObject->getCollisionShape();
	delete FieldGhostObject;
}


// returns 0 when field should be removed from the queue
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
			if (collidedPlayer->GetObjectId() != fieldOwner->GetObjectId())
			{
				printf("player is taking damage from Damage Field\n");
				if (collidedPlayer->takeDamage(this->fieldDamage))
				{
					printf("Player is dead!\n");
					//TODO Handle Player Death: send player death to client...  Maybe handle this on player
				}
			}
		}
	}
	return fieldTtl;
}