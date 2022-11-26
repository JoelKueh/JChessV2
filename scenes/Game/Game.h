#pragma once

#include <ncurses.h>
#include <string>
#include "../StartMenu/StartMenu.h"
#include "../Scene.h"
#include "Player/Player.h"
#include "ChessBoard.h"
#include "Player/PlayerLocal.h"
#include "Player/PlayerAI.h"
#include "Player/PlayerRemote.h"
#include "Player/PlayerTheFish.h"

class Game : public Scene
{
public:
  enum player_type
  {
    local = 0,
    remote = 1,
    AI = 2,
    TheFish = 3,
  };

  Game(int white, int black, std::string *time_str);
  int update();
  Scene *create_new();

private:
  Player *p_white;
  Player *p_black;
  ChessBoard *board;
  int increment;

  Player *create_player(int type);
  void parse_time_str(std::string *time_str);

  void init_scene();
  void init_board_win();
  WINDOW *board_win;
  
  void draw_chess_board(char** board);
  void draw_top_line();
	void draw_piece_line(int row_num, char* row_arr);
	void draw_other_line();
	void draw_bottom_line();
  
  void init_timer_win();
  void init_input_win();
};