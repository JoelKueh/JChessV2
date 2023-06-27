#include "BoardRep.h"
#include "tables/move_tables.h"

CB::BoardRep::BoardRep()
{
	Tables::gen_move_tables();

	char init_board[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	write_fen(init_board);
}

CB::BoardRep::BoardRep(const char fen[])
{
	Tables::gen_move_tables();

	write_fen(fen);
}

void CB::BoardRep::make(const Move &move)
{
	move_history.push(move);

	state_history.push(state_history.top());
	board_state_extra *new_state = &state_history.top();

	unsigned int flags = move.get_flags();
	unsigned int to = move.get_to();
	unsigned int from = move.get_from();

	switch (flags)
	{
	case Move::QUIETS: {
		new_state->set_cap_piece(EMPTY);
		write_piece(white_turn, get_pid(from), to);
		delete_piece(from);
		break;
	}
	case Move::CAPTURE: {
		new_state->set_cap_piece(get_pid(to));
		write_piece(white_turn, get_pid(from), to);
		delete_piece(from);

		if (to == W_KSC_ROOK)
			new_state->remove_ksc_right(true);
		else if (to == W_QSC_ROOK)
			new_state->remove_qsc_right(true);
		else if (to == B_KSC_ROOK)
			new_state->remove_ksc_right(false);
		else if (to == B_QSC_ROOK)
			new_state->remove_qsc_right(false);

		break;
	}
	case Move::DOUBLE_PAWN_PUSH: {
		new_state->set_enp(to % 8);
		write_piece(white_turn, PAWN, to);
		delete_piece(from);
		break;
	}
	case Move::KING_SIDE_CASTLE: {
		new_state->remove_castle_rights(white_turn);
		new_state->set_cap_piece(EMPTY);

		from = white_turn ? WKING_INIT_SQ : BKING_INIT_SQ;
		to = from + 2;
		int rook_from = from + 3;
		int rook_to = from + 1;
		delete_piece(from);
		delete_piece(rook_from);
		write_piece(white_turn, ROOK, rook_to);
		write_piece(white_turn, KING, to);

		break;
	}
	case Move::QUEEN_SIDE_CASTLE: {
		new_state->remove_castle_rights(white_turn);
		new_state->set_cap_piece(EMPTY);

		from = white_turn ? WKING_INIT_SQ : BKING_INIT_SQ;
		to = from - 2;
		int rook_from = from - 4;
		int rook_to = from - 1;
		delete_piece(from);
		delete_piece(rook_from);
		write_piece(white_turn, ROOK, rook_to);
		write_piece(white_turn, KING, to);

		break;
	}
	case Move::ENPASSANT: {
		new_state->set_cap_piece(PAWN);
		delete_piece(from);
		delete_piece(to + 8 * (white_turn ? 1 : -1));
		write_piece(white_turn, PAWN, to);
		break;
	}
	case Move::KNIGHT_PROMO: {
		new_state->set_cap_piece(EMPTY);
		delete_piece(from);
		write_piece(white_turn, KNIGHT, to);
		break;
	}
	case Move::BISHOP_PROMO: {
		new_state->set_cap_piece(EMPTY);
		delete_piece(from);
		write_piece(white_turn, BISHOP, to);
		break;
	}
	case Move::ROOK_PROMO: {
		new_state->set_cap_piece(EMPTY);
		delete_piece(from);
		write_piece(white_turn, ROOK, to);
		break;
	}
	case Move::QUEEN_PROMO: {
		new_state->set_cap_piece(EMPTY);
		delete_piece(from);
		write_piece(white_turn, QUEEN, to);
		break;
	}
	case Move::KNIGHT_PROMO_CAPTURE: {
		new_state->set_cap_piece(get_pid(to));
		delete_piece(from);
		write_piece(white_turn, KNIGHT, to);
		break;
	}
	case Move::BISHOP_PROMO_CAPTURE: {
		new_state->set_cap_piece(get_pid(to));
		delete_piece(from);
		write_piece(white_turn, BISHOP, to);
		break;
	}
	case Move::ROOK_PROMO_CAPTURE: {
		new_state->set_cap_piece(get_pid(to));
		delete_piece(from);
		write_piece(white_turn, ROOK, to);
		break;
	}
	case Move::QUEEN_PROMO_CAPTURE: {
		new_state->set_cap_piece(get_pid(to));
		delete_piece(from);
		write_piece(white_turn, QUEEN, to);
		break;
	}
	}

	white_turn = !white_turn;
}

void CB::BoardRep::unmake()
{
	white_turn = !white_turn;

	Move &move = move_history.top(); 
	board_state_extra &state = state_history.top();

	unsigned int from = move.get_from();
	unsigned int to = move.get_to();
	unsigned int flags = move.get_flags();

	switch (flags)
	{
	case Move::DOUBLE_PAWN_PUSH:
	case Move::QUIETS: {
		write_piece(white_turn, get_pid(from), to);
		delete_piece(from);
		break;
	}
	case Move::CAPTURE: {
		write_piece(white_turn, get_pid(from), to);
		write_piece(white_turn, state.get_pid_col(), from);
		break;
	}
	case Move::KING_SIDE_CASTLE: {
		from = white_turn ? WKING_INIT_SQ : BKING_INIT_SQ;
		to = from + 2;
		int rook_from = from + 3;
		int rook_to = from + 1;
		write_piece(white_turn, KING, from);
		write_piece(white_turn, ROOK, rook_from);
		delete_piece(rook_to);
		delete_piece(to);

		break;
	}
	case Move::QUEEN_SIDE_CASTLE: {
		from = white_turn ? WKING_INIT_SQ : BKING_INIT_SQ;
		to = from - 2;
		int rook_from = from - 4;
		int rook_to = from - 1;
		write_piece(white_turn, KING, from);
		write_piece(white_turn, ROOK, rook_from);
		delete_piece(to);
		delete_piece(rook_to);

		break;
	}
	case Move::ENPASSANT: {
		delete_piece(to);
		write_piece(!white_turn, PAWN, (to + 8 * (white_turn ? 1 : -1)));
		write_piece(white_turn, PAWN, from);
		break;
	}
	case Move::KNIGHT_PROMO:
	case Move::BISHOP_PROMO:
	case Move::ROOK_PROMO:
	case Move::QUEEN_PROMO: {
		delete_piece(to);
		write_piece(white_turn, PAWN, from);
		break;
	}
	case Move::KNIGHT_PROMO_CAPTURE:
	case Move::BISHOP_PROMO_CAPTURE:
	case Move::ROOK_PROMO_CAPTURE:
	case Move::QUEEN_PROMO_CAPTURE: {
		write_piece(white_turn, state.get_pid_col(), to);
		write_piece(white_turn, PAWN, from);
		break;
	}
	}
}

void CB::BoardRep::write_piece(bool is_white, int pid, int sq)
{
	int old_pid = get_pid(sq);
	bool old_is_white = (1ULL << sq) & bb.color[1];
	bb.piece[old_is_white][old_pid] &= ~(1ULL << sq);
	bb.color[old_is_white] &= ~(1ULL << sq);

	bb.piece[is_white][pid] |= 1ULL << sq;
	bb.color[is_white] |= 1ULL << sq;
	bb.occupied |= 1ULL << sq;

	mailbox[sq] = pid;
}

void CB::BoardRep::delete_piece(int sq)
{
	int old_pid = get_pid(sq);
	bool old_is_white = (1ULL << sq) & bb.color[1];
	bb.piece[old_is_white][old_pid] &= ~(1ULL << sq);
	bb.color[old_is_white] &= ~(1ULL << sq);

	mailbox[sq] = EMPTY;
}

std::vector<CB::Move> *CB::BoardRep::gen_move_list()
{
	std::vector<Move> *move_list;

	if (!tables_valid)
		update_all();

	append_simple_moves(move_list);
	append_castle_moves(move_list);
	append_enp_moves(move_list);
	append_promos(move_list);

	return move_list;
}

void CB::BoardRep::append_simple_moves(std::vector<Move> *move_list)
{
	uint64_t pieces = bb.color[white_turn];
	// Take out the pawns that are on the opposing sides pawn home row
	// as their next move will be a promotion and should be handled seperately
	pieces ^= bb.piece[white_turn][PAWN] & (white_turn ? BB_B_PAWN_HOME : BB_W_PAWN_HOME);
	while (pieces) {
		int sq = pop_rbit(pieces);
		uint64_t simple = get_legal_mv_mask(sq);
		while (simple) {
			int target = pop_rbit(simple);
			move_list->push_back(format_mv(target, sq));
		}
	}
}

void CB::BoardRep::append_castle_moves(std::vector<Move> *move_list)
{
	if (ksc_legal())
		move_list->push_back(Move(0, 0, Move::KING_SIDE_CASTLE));

	if (qsc_legal())
		move_list->push_back(Move(0, 0, Move::QUEEN_SIDE_CASTLE));
}

bool CB::BoardRep::ksc_legal()
{
	if (!state_history.top().get_ksc_right(white_turn))
		return false;

	if ((white_turn ? W_KSC_OCC_MASK : B_KSC_OCC_MASK) & (bb.occupied | threats))
		return false;

	return true;
}

bool CB::BoardRep::qsc_legal()
{
	if (!state_history.top().get_qsc_right(white_turn))
		return false;

	if ((white_turn ? W_QSC_OCC_MASK : B_QSC_OCC_MASK) & (bb.occupied | threats))
		return false;

	return true;
}

void CB::BoardRep::append_enp_moves(std::vector<Move> *move_list)
{
	if (!state_history.top().enp_availiable())
		return;

	int enp_sq = (white_turn ? LOWEST_B_ENP_ROW_SQ : LOWEST_W_ENP_ROW_SQ)
		+ state_history.top().get_enp_col();
	uint64_t enp_sources = Tables::read_patk(!white_turn, enp_sq) & bb.piece[white_turn][PAWN];
	while (enp_sources) {
		int sq = pop_rbit(enp_sources);
		move_list->push_back(Move(enp_sq, sq, Move::ENPASSANT));
	}
}

void CB::BoardRep::append_promos(std::vector<Move> *move_list)
{
	uint64_t pawns = bb.piece[white_turn][PAWN] & (white_turn ? BB_B_PAWN_HOME : BB_W_PAWN_HOME);
	while (pawns) {
		int direction = white_turn ? -1 : 1;
		int sq = pop_rbit(pawns);

		int push_target = sq + 8 * direction;
		if (mailbox[push_target] == EMPTY) {
			move_list->push_back(Move(push_target, sq, Move::KNIGHT_PROMO));
			move_list->push_back(Move(push_target, sq, Move::BISHOP_PROMO));
			move_list->push_back(Move(push_target, sq, Move::ROOK_PROMO));
			move_list->push_back(Move(push_target, sq, Move::QUEEN_PROMO));
		}

		uint64_t cap_targets = Tables::read_patk(white_turn, sq);
		cap_targets &= bb.color[!white_turn];
		while (cap_targets) {
			int target = pop_rbit(cap_targets);
			move_list->push_back(Move(target, sq, Move::KNIGHT_PROMO_CAPTURE));
			move_list->push_back(Move(target, sq, Move::BISHOP_PROMO_CAPTURE));
			move_list->push_back(Move(target, sq, Move::ROOK_PROMO_CAPTURE));
			move_list->push_back(Move(target, sq, Move::QUEEN_PROMO_CAPTURE));
		}
	}
}

void CB::BoardRep::get_mv_set(move_set *mask, int sq)
{
	int pid = get_pid(sq);

	if (!tables_valid)
		update_all();

	mask->push = get_legal_mv_mask(sq);
	mask->cap = mask->push & bb.color[!white_turn];
	mask->push ^= mask->cap;

	if (pid == PAWN) {
		mask->special = get_enp_mask(sq);
		mask->promo = (mask->push | mask->cap) & (white_turn ? BB_TOP_ROW : BB_BOTTOM_ROW);
		mask->push &= ~mask->promo;
		mask->cap &= ~mask->promo;
	} else if (pid == KING) {
		mask->special = get_castle_mask();
		mask->promo = 0;
	} else {
		mask->special = 0;
		mask->promo = 0;
	}
}

void CB::BoardRep::update_all()
{
	tables_valid = true;

	update_checks();
	update_pins();
}

void CB::BoardRep::update_checks()
{
	threats = pawn_smear(white_turn);

	uint64_t pieces = bb.color[white_turn] ^ bb.piece[white_turn][PAWN];
	while (pieces) {
		int sq = pop_rbit(pieces);
		threats |= get_pseudo_mv_mask(sq);
	}

	if (!(bb.piece[white_turn][KING] & threats)) {
		checkers = 0;
		return;
	}

	int king_sq = peek_rbit(bb.piece[white_turn][KING]);
	checkers = Tables::read_patk(white_turn, king_sq) & bb.piece[!white_turn][PAWN];
	checkers |= Tables::read_natk(king_sq) & bb.piece[!white_turn][KNIGHT];
	checkers |= Tables::read_batk(king_sq, bb.occupied)
		& (bb.piece[!white_turn][BISHOP] | bb.piece[!white_turn][QUEEN]);
	checkers |= Tables::read_ratk(king_sq, bb.occupied)
		& (bb.piece[!white_turn][ROOK] | bb.piece[!white_turn][QUEEN]);
	// Helpful reminder that a king can never check another king
}

uint64_t CB::BoardRep::pawn_smear(bool is_white)
{
	uint64_t output;

	if (is_white) {
		output = (bb.piece[false][PAWN] << 9) & ~BB_R_COL; 
		output |= (bb.piece[false][PAWN] << 7) & ~BB_L_COL;
		return output;
	}

	output = (bb.piece[false][PAWN] >> 7) & ~BB_R_COL; 
	output |= (bb.piece[false][PAWN] >> 9) & ~BB_L_COL;
	return output;
}

void CB::BoardRep::update_pins()
{
	pins[9] = 0;
	int king_sq = peek_rbit(bb.piece[white_turn][KING]);

	int i = 0;
	uint64_t pinner = xray_ratk(bb.occupied, bb.color[white_turn], king_sq)
		& (bb.piece[white_turn][ROOK] | bb.piece[white_turn][QUEEN]);
	while (pinner) {
		int sq = pop_rbit(pinner);
		pins[i] = Tables::tf_table[sq][king_sq];
		pins[9] ^= pins[i++];
	}

	pinner = xray_batk(bb.occupied, bb.color[white_turn], king_sq)
		& (bb.piece[white_turn][BISHOP] | bb.piece[white_turn][QUEEN]);
	while (pinner) {
		int sq = pop_rbit(pinner);
		pins[i] = Tables::tf_table[sq][king_sq];
		pins[9] ^= pins[i++];
	}

	pins[9] &= ~(1ULL << king_sq);
}

uint64_t CB::BoardRep::xray_ratk(uint64_t occ, uint64_t blockers, int sq)
{
	uint64_t attacks = Tables::read_ratk(sq, occ);
	blockers &= attacks;
	return attacks ^ Tables::read_ratk(sq ^ blockers, occ);
}

uint64_t CB::BoardRep::xray_batk(uint64_t occ, uint64_t blockers, int sq)
{
	uint64_t attacks = Tables::read_batk(sq, occ);
	blockers &= attacks;
	return attacks ^ Tables::read_ratk(sq ^ blockers, occ);
}

inline int CB::BoardRep::get_pid(int sq)
{
	return mailbox[sq];
}

uint64_t CB::BoardRep::get_pseudo_mv_mask(int sq)
{
	int pid = get_pid(sq);

	switch (pid) {
		case PAWN: {
				   return pawn_moves(sq);
			   }
		case KNIGHT: {
				     return Tables::read_natk(sq);
			     }
		case BISHOP: {
				     return Tables::read_batk(sq, bb.occupied);
			     }
		case ROOK: {
				   return Tables::read_ratk(sq, bb.occupied);
			   }
		case QUEEN: {
				    uint64_t moves = Tables::read_ratk(sq, bb.occupied)
					    | Tables::read_batk(sq, bb.occupied);
				    return moves;
			    }
		case KING: {
				   return Tables::read_katk(sq);
			   }
		default: {
				 return 0;
			 }
	}
}

uint64_t CB::BoardRep::get_legal_mv_mask(int sq)
{
	int pid = get_pid(sq);
	uint64_t moves = get_pseudo_mv_mask(sq);

	if (pid == KING) {
		king_mv_adjust(sq, moves);
		return moves;
	}

	uint64_t legal_mask = get_pin_mask(sq);
	legal_mask &= check_blocks;

	return moves & legal_mask;
}

void CB::BoardRep::king_mv_adjust(int sq, uint64_t &moves)
{
	moves &= ~threats;
}

uint64_t CB::BoardRep::get_pin_mask(int sq)
{
	if (!((1ULL << sq) & pins[9]))
		return BB_FULL;	

	int i = -1;
	while (!((1ULL << sq) & pins[++i]));

	return pins[i];
}

void CB::BoardRep::update_check_blocks()
{
	if (!checkers) {
		check_blocks = BB_FULL;
		return;
	}

	if (popcnt(checkers) != 1) {
		check_blocks = 0;
		return;
	}

	int king_sq = peek_rbit(bb.piece[white_turn][KING]);
	int checkr_sq = peek_rbit(checkers);
	check_blocks = checkers | Tables::tf_table[king_sq][checkers];
}

