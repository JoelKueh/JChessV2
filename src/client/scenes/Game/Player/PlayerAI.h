#pragma once

#include "Player.h"

class PlayerAI : public Player
{
private:

public:
  PlayerAI();
  virtual int get_status();
  virtual Move *get_move();
  virtual void send_board();
  virtual ~PlayerAI();
};