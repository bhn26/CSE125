#include "BlastField.h"
#include "Player.h"
#include "Entity.h"
#include "FireRateReset.h"

BlastField::BlastField(bool nonFriendly, int stun, int magnitude, int ttl, int damage, btVector3* origin, btCollisionShape* fieldshape, int team_id, btDiscreteDynamicsWorld* curworld) : FieldObject(origin, fieldshape, team_id, curworld)
{
	this->stun = stun;
	this->fieldTtl = ttl;
	this->fieldDamage = damage;
	this->nonFriendly = nonFriendly;
	this->magnitude = magnitude;
    this->type = FieldType::EXPLOSION;
}
BlastField::~BlastField()
{
	this->curWorld->removeCollisionObject(FieldGhostObject);
	delete FieldGhostObject->getCollisionShape();
	delete FieldGhostObject;
}

int BlastField::handleField()
{
	fieldTtl--;
	int numOverlap = FieldGhostObject->getNumOverlappingObjects();
	for (int i = 0; i < numOverlap; i++)
	{
		btRigidBody *pRigidBody = dynamic_cast<btRigidBody *>(FieldGhostObject->getOverlappingObject(i));
		if (pRigidBody && pRigidBody->getUserIndex() != ClassId::OBSTACLE && pRigidBody->getUserIndex() != ClassId::FIELD)
		{
			Entity* ent = (Entity *)pRigidBody->getUserPointer();

			if (ent->GetClassId() == ClassId::PLAYER)
			{
				Player* collidedPlayer = (Player*)ent;
				if (collidedPlayer->GetTeamId() != team_id || nonFriendly){ // If the player is an enemy or if this field is nonfriendly
					collidedPlayer->takeDamage(this->fieldDamage, FireRateReset::instance()->currentWorldTick);
					collidedPlayer->SetStun(stun);
					
				}
				else {
					break;
				}
			}

			// if we haven't already blasted this kid
			if(blasted.find(std::pair<int,int>(ent->GetClassId(), ent->GetObjectId())) == blasted.end())
			{ 
				btTransform field = FieldGhostObject->getWorldTransform();
				btVector3 fieldCenter = field.getOrigin();

				btVector3 playerCenter = ent->GetEntityPosition();
				btVector3 blastDirection = playerCenter - fieldCenter;
				blastDirection = blastDirection.normalize();
				btVector3 velocity = magnitude * blastDirection;
				blastDirection.setY(ydirection);

				ent->Move(&velocity);
				std::pair<int,int> entpair = std::pair<int, int>(ent->GetClassId(), ent->GetObjectId());
				blasted.insert(std::pair<std::pair<int,int>,int>(entpair, 1));
			}


		}
	}

	if (fieldTtl > 0)
		return 0;
	else
		return 1;
}