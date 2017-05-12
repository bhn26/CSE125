#include "PlayerModifierCollision.h"

PlayerModifierCollision::PlayerModifierCollision(Player* shooter)
{
	this->shooter = shooter;
}

PlayerModifierCollision::~PlayerModifierCollision() {}

// Does not consider what collided with it
bool PlayerModifierCollision::HandleBulletCollision(unsigned int world_tick, Entity* collidee)
{
	if(bullet == nullptr)
		return true;
	
	// stay in bounds
	btVector3 bul_pos = bullet->GetEntityPosition();
	if(bul_pos.getX() > Constants::worldWidth - 5)
		bul_pos.setX(Constants::worldWidth - 5);
	if(bul_pos.getX() < (Constants::worldWidth * (-1)) + 5)
		bul_pos.setX((Constants::worldWidth * (-1)) + 5);

	if (bul_pos.getZ() > Constants::worldWidth - 5)
			bul_pos.setZ(Constants::worldWidth - 5);
	if (bul_pos.getZ() < (Constants::worldWidth * (-1)) + 5)
		bul_pos.setZ((Constants::worldWidth * (-1)) + 5);

	if (bul_pos.getY() < 2)
		bul_pos.setY(2);
		
	shooter->SetEntityPosition(&bul_pos);

	// This is pretty much for the teleport gun right now
	return true;
}