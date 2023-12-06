
#ifndef H_NET_UTILS
#define H_NET_UTILS

#include "../../common/CB_2_0/BoardRep.h"

#include <iostream>

int split_command(char command[], char ***words);
void print_board(const CB::BoardRep &board);
CB::Move algbr_to_move(CB::BoardRep &board, std::string &algebraic);
std::string move_to_algbr(CB::BoardRep &board, CB::Move move);

#endif
