#include "Player.h"

Player::Player()
{
  move_buffer = new Move();
}

Player::~Player()
{
	delete move_buffer;
}