#include "PlayerModifierCollision.h"

PlayerModifierCollision::PlayerModifierCollision(Player* shooter)
{
	this->shooter = shooter;
}

PlayerModifierCollision::~PlayerModifierCollision() {}

bool PlayerModifierCollision::HandleBulletCollision(unsigned int world_tick)
{
	if(bullet == nullptr)
		return true;
	
	btVector3 bul_pos = bullet->GetEntityPosition();
	shooter->SetEntityPosition(&bul_pos);

	// This is pretty much for the teleport gun right now
	return true;
}