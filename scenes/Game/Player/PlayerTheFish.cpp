#include "PlayerTheFish.h"

PlayerTheFish::PlayerTheFish() = default;

int PlayerTheFish::get_status()
{
  return P_OK;
}

Player::move *PlayerTheFish::get_move()
{
  last_move.end_x = 0;
  last_move.end_y = 0;
  last_move.start_x = 0;
  last_move.start_y = 0;

  return &last_move;
}

void PlayerTheFish::send_board()
{
  
}