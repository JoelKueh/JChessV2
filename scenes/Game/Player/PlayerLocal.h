#pragma once

#include "Player.h"

class PlayerLocal : public Player
{
private:

public:
  PlayerLocal();
  int get_status();
  move* get_move();
  void send_board();
};