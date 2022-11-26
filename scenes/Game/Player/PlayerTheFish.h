#pragma once

#include "Player.h"

class PlayerTheFish : public Player
{
private:

public:
  PlayerTheFish();
  int get_status();
  move* get_move();
  void send_board();
};