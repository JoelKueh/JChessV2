#ifndef H_CHESS_INATOR
#define H_CHESS_INATOR

#include "../CB_2_0/BoardRep.h"
#include <limits>
#include <thread>

namespace AI
{

class ChessInator : CB::BoardRep
{
public:
	ChessInator() = default;

	CB::Move best_move();

	~ChessInator() = default;
private:

	double alpha_beta_min(double alpha, double beta, int depth);
	double alpha_beta_max(double alpha, double beta, int depth);

	double evaluate();
};

}

#endif
