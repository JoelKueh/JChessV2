#pragma once
#include "Player.h"

class PlayerRemote : public Player
{
private:

public:
  PlayerRemote();
  int get_status();
  move* get_move();
  void send_board();
};