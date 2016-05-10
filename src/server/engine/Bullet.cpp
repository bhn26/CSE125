

#include "Bullet.h"

Bullet::Bullet(int objectid, int playerid, int teamid)
{
	this->id = objectid;
	this->playerId = playerid;
	this->teamId = teamid;
}

Bullet::~Bullet()
{
}

int Bullet::GetObjectId()
{
	return id;
}

int Bullet::GetPlayerId()
{
	return playerId;
}

int Bullet::GetTeamId()
{
	return teamId;
}