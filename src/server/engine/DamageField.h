#pragma once
#include "FieldObject.h"

class FieldObject;

// A field that does damage over time
class DamageField : public FieldObject
{
protected:
    int m_fieldTtl = 0;
    int m_fieldDamage = 0;

public:
    DamageField(int ttl,
                int damage,
                const btVector3& origin,
                btCollisionShape* fieldshape,
                int team_id,
                btDiscreteDynamicsWorld* curworld);
    virtual ~DamageField();
    int HandleField() override;
};