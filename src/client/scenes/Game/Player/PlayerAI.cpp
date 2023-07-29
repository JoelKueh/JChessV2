#include "PlayerAI.h"

PlayerAI::PlayerAI() = default;

int PlayerAI::get_status()
{
  return P_OK;
}

Move *PlayerAI::get_move()
{
  return move_buffer;
}

void PlayerAI::send_board()
{
  
}

PlayerAI::~PlayerAI() = default;