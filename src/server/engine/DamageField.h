#pragma once
#include "FieldObject.h"


class DamageField : public FieldObject
{
protected:
	int fieldDamage;

public:
	DamageField(int ttl, int damage, btVector3* origin, btCollisionShape* fieldshape, Entity* fieldowner, btDiscreteDynamicsWorld* curworld);
	virtual ~DamageField();
	int handleField();
};