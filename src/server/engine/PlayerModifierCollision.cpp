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
	
	// stay in bounds
	btVector3 bul_pos = bullet->GetEntityPosition();
	if(bul_pos.getX() > WORLD_WIDTH - 5)
		bul_pos.setX(WORLD_WIDTH - 5);
	if(bul_pos.getX() < (WORLD_WIDTH * (-1)) + 5)
		bul_pos.setX((WORLD_WIDTH * (-1)) + 5);

	if (bul_pos.getZ() > WORLD_WIDTH - 5)
			bul_pos.setZ(WORLD_WIDTH - 5);
	if (bul_pos.getZ() < (WORLD_WIDTH * (-1)) + 5)
		bul_pos.setZ((WORLD_WIDTH * (-1)) + 5);
		
	shooter->SetEntityPosition(&bul_pos);

	// This is pretty much for the teleport gun right now
	return true;
}