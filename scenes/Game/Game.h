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
  virtual void init();
  virtual int update();
  virtual Scene *create_new();
  virtual ~Game();

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
  WINDOW *timer_win;
  WINDOW *input_win;
  
  void update_pieces(char **board);
  void update_piece_row(char *row);
  void update_piece_col(char *col);
  void update_piece_square(char square);
  void draw_chess_board();
  void draw_top_line();
	void draw_piece_line();
	void draw_other_line();
	void draw_bottom_line();
  
  void init_timer_win();
  void init_input_win();
};