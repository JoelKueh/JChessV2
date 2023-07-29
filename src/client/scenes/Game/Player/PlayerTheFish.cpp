#include "PlayerTheFish.h"

PlayerTheFish::PlayerTheFish() = default;

int PlayerTheFish::get_status()
{
  return P_OK;
}

Move *PlayerTheFish::get_move()
{
  return move_buffer;
}

void PlayerTheFish::send_board()
{
  
}

PlayerTheFish::~PlayerTheFish() = default;