#pragma once
#include "FieldObject.h"

class FieldObject;

// A field that does damage over time
class DamageField : public FieldObject
{
protected:
	int fieldTtl;
	int fieldDamage;

public:
	DamageField(int ttl, int damage, btVector3* origin, btCollisionShape* fieldshape, int team_id, btDiscreteDynamicsWorld* curworld);
	virtual ~DamageField();
	int handleField();
};