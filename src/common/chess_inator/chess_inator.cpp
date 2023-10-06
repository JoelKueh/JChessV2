#include "chess_inator.h"

CB::Move AI::ChessInator::best_move()
{
	if (white_turn)
		alpha_beta_max(std::numeric_limits<double>::infinity(),
				-std::numeric_limits<double>::infinity(), 6);
	else
		alpha_beta_min(std::numeric_limits<double>::infinity(),
				-std::numeric_limits<double>::infinity(), 6);

	return CB::Move();
}

double AI::ChessInator::alpha_beta_min(double alpha, double beta, int depth)
{
	if (depth == 0)
		return evaluate();
	CB::MoveList moves;
	gen_move_list(&moves);
	double score;
	for (int i = 0; i < moves.size(); ++i) {
		make(moves.at(i));
		score = alpha_beta_max(alpha, beta, depth - 1);
		unmake();
		if (score >= beta)
			return beta;
		if (score > alpha)
			alpha = score;
	}
	return alpha;
}

double AI::ChessInator::alpha_beta_max(double alpha, double beta, int depth)
{
	if (depth == 0)
		return evaluate();
	CB::MoveList moves;
	gen_move_list(&moves);
	double score;
	for (int i = 0; i < moves.size(); ++i) {
		make(moves.at(i));
		score = alpha_beta_min(alpha, beta, depth - 1);
		unmake();
		if (score <= alpha)
			return alpha;
		if (score < beta)
			beta = score;
	}
	return beta;
}

double AI::ChessInator::evaluate()
{
	// Add up piece values
	// While you're at it, you really need to fix popcnt. It's not
	// compiling to the correct processor instruction.
	int eval = CB::popcnt(bb.piece[true][CB::pid::PAWN]);
	eval += 3 * CB::popcnt(bb.piece[true][CB::pid::KNIGHT]);
	eval += 3 * CB::popcnt(bb.piece[true][CB::pid::BISHOP]);
	eval += 5 * CB::popcnt(bb.piece[true][CB::pid::ROOK]);
	eval += 9 * CB::popcnt(bb.piece[true][CB::pid::QUEEN]);
}

