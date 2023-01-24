#pragma once

#include <bitset>

class Move
{
private:
  std::bitset<16> move_raw;

public:
  struct square
  {
    int x;
    int y;
  };

  Move();
  void set_from(int x, int y);
  void set_to(int x, int y);
  square *get_from();
  square *get_to();
  int get_from_x();
};