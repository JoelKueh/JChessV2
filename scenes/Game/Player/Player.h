#pragma once

#include "Player.h"
#include <bitset>

class Player
{
public:
  struct move
  {
    bool move_ready = false;
    short start_x = 0;
    short start_y = 0;
    short end_x = 0;
    short end_y = 0;
  }; move last_move;

  enum status
  {
    P_OK = 0,
    P_DISCONNECTED = 1,
    P_TERMINATED = 2,
  };

  int time;

  Player();
  virtual int get_status() = 0;
  virtual move* get_move() = 0;
  virtual void send_board() = 0;
};