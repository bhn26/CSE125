#pragma once
#include "FieldObject.h"
#include <map>

// a blast field that does damage and knocks back
class BlastField : public FieldObject {
protected:
	int fieldTtl;
	int fieldDamage;
	int magnitude;     // magnitude in which the blast will knock back
	int stun;
	bool nonFriendly;  // Whether or not this field friendly fires, true if it does

	static int const ydirection = 20; // the y direction magnitude to launch the player

	std::map<std::pair<int,int>, int > blasted; // list of entities that already got blasted by the velocity
	
public:
	BlastField(bool nonFriendly, int stun, int magnitude, int ttl, int damage, btVector3* origin, btCollisionShape* fieldshape, int team_id, btDiscreteDynamicsWorld* curworld);
	virtual ~BlastField();
	int handleField();
};