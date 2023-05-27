#include "PlayerLocal.h"

PlayerLocal::PlayerLocal() = default;

int PlayerLocal::get_status()
{
  return P_OK;
}

Move *PlayerLocal::get_move()
{
  return move_buffer;
}

void PlayerLocal::send_board()
{

}

PlayerLocal::~PlayerLocal() = default;