#include "Player.h"

Player::Player(int id, PosInfo pos) {
	this->id = id;
	this->position = pos;
}

Player::~Player() {

}

void Player::Move(int direction) {
	position.direction = direction;

	switch (direction) {
	case MOVE_FORWARD: 
		position.y ++;
		break;
	case MOVE_BACKWARD:
		position.y--;
		break;
	case MOVE_LEFT:
		position.x++;
		break;
	case MOVE_RIGHT:
		position.x--;
		break;
	}
}

void Player::Rotate(float v_rotation, float h_rotation) {
	position.v_rotation = v_rotation;
	position.h_rotation = h_rotation;
}