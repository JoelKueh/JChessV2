#pragma once

#include "../Move.h"
#include <bitset>
#include <chrono>

class Player
{
protected:
  Move *move_buffer;

public:
  enum status
  {
    P_OK = 0,
    P_DISCONNECTED = 1,
    P_TERMINATED = 2,
  };

  ulong time;

  Player();
  virtual int get_status() = 0;
  virtual Move *get_move() = 0;
  virtual void send_board() = 0;
  virtual ~Player();
};