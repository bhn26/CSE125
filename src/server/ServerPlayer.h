#pragma once

#include <BulletPhysics\btBulletDynamicsCommon.h>
#include <BulletPhysics\btBulletCollisionCommon.h>

class ServerPlayer
{
	btDiscreteDynamicsWorld* curWorld;
	btRigidBody* playerRigidBody;
	int jumpSem;
	int playerId;

public:

	ServerPlayer(int id, btDiscreteDynamicsWorld* physicsWorld);

	~ServerPlayer();

	// Moves player by changing velocity according to current orientation
	void movePlayer(btVector3 * changeVelocity);

	// Makes the player jump
	void jumpPlayer();

	// Rotates player according to mouse move
	void rotatePlayer();

	// Return current positioning of Player
	btVector3 getPlayerPosition();

	// Return current rotation matrix of Player
	btMatrix3x3 getPlayerRotation();

};