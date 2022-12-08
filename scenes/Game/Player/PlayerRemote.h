#pragma once
#include "Player.h"

class PlayerRemote : public Player
{
private:

public:
  PlayerRemote();
  virtual int get_status();
  virtual Move* get_move();
  virtual void send_board();
  virtual ~PlayerRemote();
};