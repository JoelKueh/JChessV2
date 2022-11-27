#include "PlayerLocal.h"

PlayerLocal::PlayerLocal() = default;

int PlayerLocal::get_status()
{
  return P_OK;
}

Player::move *PlayerLocal::get_move()
{
  last_move.end_x = 0;
  last_move.end_y = 0;
  last_move.start_x = 0;
  last_move.start_y = 0;

  return &last_move;
}

void PlayerLocal::send_board()
{

}