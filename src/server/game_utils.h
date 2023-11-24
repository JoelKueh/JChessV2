#ifndef H_GAME_UTILS
#define H_GAME_UTILS

#include "server_defs.h"
#include "../common/CB_2_0/BoardRep.h"

CB::Move algbr_to_move(CB::BoardRep &board, std::string &algebraic);

#endif /* H_GAME_UTILS */
