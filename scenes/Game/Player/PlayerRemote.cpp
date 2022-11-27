#include "PlayerRemote.h"

PlayerRemote::PlayerRemote() = default;

int PlayerRemote::get_status()
{
  return P_OK;
}

Player::move *PlayerRemote::get_move()
{
  last_move.end_x = 0;
  last_move.end_y = 0;
  last_move.start_x = 0;
  last_move.start_y = 0;

  return &last_move;
}

void PlayerRemote::send_board()
{
  
}