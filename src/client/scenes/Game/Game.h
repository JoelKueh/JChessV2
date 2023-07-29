#pragma once

#include <ncurses.h>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include "../StartMenu/StartMenu.h"
#include "../Scene.h"
#include "Player/Player.h"
#include "../../../common/CB_2_0/BoardRep.h"
#include "Player/PlayerLocal.h"
#include "Player/PlayerAI.h"
#include "Player/PlayerRemote.h"
#include "Player/PlayerTheFish.h"
#include "UI/GameCLI.h"
#include "UI/GameGUI.h"

extern options my_options;

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
  short increment;
  std::vector<Move> move_history;

  Player *p_white;
  Player *p_black;
  CB::BoardRep *board;
  GameUI *UI;

  unsigned long long last_time = 0;

  Player *create_player(int type);
  void parse_time_str(std::string *time_str);
  void update_player_time();
  void handle_player_move();

  int last_selected_piece = -1;
};
