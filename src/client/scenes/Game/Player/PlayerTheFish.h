#pragma once

#include "Player.h"

class PlayerTheFish : public Player
{
private:

public:
  PlayerTheFish();
  virtual int get_status();
  virtual Move* get_move();
  virtual void send_board();
  virtual ~PlayerTheFish();
};