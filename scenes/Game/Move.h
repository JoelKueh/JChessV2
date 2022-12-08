#pragma once

#include <bitset>

class Move
{
private:
  std::bitset<16> move_raw;

public:
  Move();
  void set_from();
  void set_to();
  void get_from();
  void get_to();
}