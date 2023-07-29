#include "PlayerRemote.h"

PlayerRemote::PlayerRemote() = default;

int PlayerRemote::get_status()
{
  return P_OK;
}

Move *PlayerRemote::get_move()
{
  return move_buffer;
}

void PlayerRemote::send_board()
{
  
}

PlayerRemote::~PlayerRemote() = default;