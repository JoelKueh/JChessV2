#pragma once

#include "Player.h"

class PlayerLocal : public Player
{
private:

public:
  PlayerLocal();
  virtual int get_status();
  virtual Move* get_move();
  virtual void send_board();
  virtual ~PlayerLocal();
};