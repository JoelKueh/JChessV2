#pragma once

#include "Player.h"

class Local_Player : public Player
{
  Local_Player();
  int get_status();
  move* get_move();
  
}