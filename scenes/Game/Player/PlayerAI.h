#pragma once

#include "Player.h"
#include <bitset>

class PlayerAI : public Player
{
private:

public:
  PlayerAI();
  int get_status();
  move* get_move();
  void send_board();
};