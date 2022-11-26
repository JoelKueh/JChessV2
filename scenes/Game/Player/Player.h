#pragma once

#include "Player.h"
#include <bitset>

#define P_OK 0
#define P_DISCONNECTED 1
#define P_TERMINATED 2

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

  int time;

  Player();
  virtual int get_status() = 0;
  virtual move* get_move() = 0;
  virtual void send_board() = 0;
};