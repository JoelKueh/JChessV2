#include "BoardRep.h"
#include "tables/move_tables.h"

CB::BoardRep::BoardRep()
{
	Tables::gen_move_tables();

	char init_board[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	write_fen(init_board);
}

CB::BoardRep::BoardRep(char fen[])
{
	Tables::gen_move_tables();

	write_fen(fen);
}

void CB::BoardRep::wipe_board()
{
	move_history.clear();
	state_history.clear();

	for (int i = 0; i < 2; ++i) {
		bb.color[i] = 0;
		for (int k = 0; k < 6; ++k) {
			bb.piece[i][k] = 0;
		}
	}
	bb.occupied = 0;

	for (int i = 0; i < 64; ++i) {
		mailbox[i] = EMPTY;
	}

	white_turn = true;
}

void CB::BoardRep::write_fen(const char *const fen_str)
{
	wipe_board();
	int sq = 0;
	const char *fen = fen_str;

	while (sq < 64) {
		parse_fen_main(*fen++, sq);
	}

	white_turn = (*++fen == 'w');
	++fen;

	state_history.push_back(board_state_extra());
	board_state_extra &curr = state_history.back();
	for (int i = 0; i < 4; ++i) {
		switch (*fen++) {
			case 'K': curr.set_ksc_right(true); continue;
			case 'Q': curr.set_qsc_right(true); continue;
			case 'k': curr.set_ksc_right(false); continue;
			case 'q': curr.set_qsc_right(false); continue;

			default: continue;
		}
	}
	fen++;

	if (*fen++ != '-')
		curr.set_enp(*fen++ - '0');
	fen++;

	int half_mv_clock = 10;
	while (*fen != ' ') {
		half_mv_clock *= 10;
		half_mv_clock += *fen++ - '0';
	}
	fen++;

	full_move_number = 0;
	while (*fen != ' ') {
		full_move_number *= 10;
		full_move_number += *fen++ - '0';
	}

	tables_valid = false;
}

void CB::BoardRep::parse_fen_main(const char to_parse, int &sq)
{
	switch (to_parse) {
		case 'p': write_piece(false, PAWN, sq++); return;
		case 'P': write_piece(true, PAWN, sq++); return;
		case 'n': write_piece(false, KNIGHT, sq++); return;
		case 'N': write_piece(true, KNIGHT, sq++); return;
		case 'b': write_piece(false, BISHOP, sq++); return;
		case 'B': write_piece(true, BISHOP, sq++); return;
		case 'r': write_piece(false, ROOK, sq++); return;
		case 'R': write_piece(true, ROOK, sq++); return;
		case 'q': write_piece(false, QUEEN, sq++); return;
		case 'Q': write_piece(true, QUEEN, sq++); return;
		case 'k': write_piece(false, KING, sq++); return;
		case 'K': write_piece(true, KING, sq++); return;

		case '/': return;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			  sq += to_parse - '0'; return;

		default: sq = 64; return;
	}
}

void CB::BoardRep::board_to_str(char board_str[8][8]) const
{
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			board_str[row][col] = sq_to_char(row * 8 + col);
		}
	}
}

char CB::BoardRep::sq_to_char(int sq) const
{
	// This is not null-character delimited, don't try to print it
	const char piece_arr[7] = { 'p', 'n', 'b', 'r', 'q', 'k', ' ' };
	constexpr int to_up = 'a' - 'A';

	char piece = piece_arr[get_pid(sq)];
	piece -= sq_is_white(sq) ? to_up : 0;

	return piece;
}

bool CB::BoardRep::sq_is_white(int sq) const
{
	return ((1ULL << sq) & bb.color[true]);
}

void CB::BoardRep::make(const Move &move)
{
	move_history.push_back(move);

	state_history.push_back(state_history.back());
	board_state_extra *new_state = &state_history.back();

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
	tables_valid = false;
}

void CB::BoardRep::unmake()
{
	white_turn = !white_turn;

	Move &move = move_history.back(); 
	board_state_extra &state = state_history.back();

	unsigned int from = move.get_from();
	unsigned int to = move.get_to();
	unsigned int flags = move.get_flags();

	switch (flags)
	{
	case Move::DOUBLE_PAWN_PUSH:
	case Move::QUIETS: {
		write_piece(white_turn, get_pid(to), from);
		delete_piece(to);
		break;
	}
	case Move::CAPTURE: {
		write_piece(white_turn, get_pid(to), from);
		write_piece(!white_turn, state.get_pid_col(), to);
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
		write_piece(!white_turn, state.get_pid_col(), to);
		write_piece(white_turn, PAWN, from);
		break;
	}
	}

	tables_valid = false;
	move_history.pop_back();
	state_history.pop_back();
}

void CB::BoardRep::write_piece(bool is_white, int pid, int sq)
{
	int old_pid = get_pid(sq);

	// TODO: THIS MAY BE REMOVABLE, CHECK MAY BE REDUNDANT
	if (old_pid != pid::EMPTY) {
		bool old_is_white = (1ULL << sq) & bb.color[1];
		bb.piece[old_is_white][old_pid] &= ~(1ULL << sq);
		bb.color[old_is_white] &= ~(1ULL << sq);
	}

	bb.piece[is_white][pid] |= 1ULL << sq;
	bb.color[is_white] |= 1ULL << sq;
	bb.occupied |= 1ULL << sq;

	mailbox[sq] = pid;
}

void CB::BoardRep::delete_piece(int sq)
{
	int old_pid = get_pid(sq);

	// TODO: THIS MAY BE REMOVABLE AS WELL, REDUNDANT CHECK?
	if (old_pid != pid::EMPTY) {
		bool old_is_white = (1ULL << sq) & bb.color[1];
		bb.piece[old_is_white][old_pid] &= ~(1ULL << sq);
		bb.color[old_is_white] &= ~(1ULL << sq);
		bb.occupied &= ~(1ULL << sq);
	}

	mailbox[sq] = pid::EMPTY;
}

CB::Move CB::BoardRep::format_mv(unsigned int to, unsigned int from, pid promo_piece)
{
	unsigned int flags;
	move_set mask;
	get_mv_set(&mask, from);

	if ((mask.push | mask.cap) & (1ULL << to)) {
		return format_simple_mv(to, from);
	} else if (mask.special & (1ULL << to)) {
		int row = to / 8;
		if (row == 2 || row == 5)
			return Move(from, to, Move::ENPASSANT);
		else if (row == 3 || row == 4)
			return Move(from, to, Move::DOUBLE_PAWN_PUSH);
		else if (to == W_QSC_TARGET || to == B_QSC_TARGET)
			return Move(0, 0, Move::QUEEN_SIDE_CASTLE);
		else if (to == W_KSC_TARGET || to == B_KSC_TARGET)
			return Move(0, 0, Move::KING_SIDE_CASTLE);
		return Move(Move::INVALID);
	} else if (mask.promo & (1ULL << to)) {
		return format_promo_mv(to, from, promo_piece);
	} else {
		return Move(Move::INVALID);
	}
}

CB::Move CB::BoardRep::format_simple_mv(unsigned int to, unsigned int from)
{
	unsigned int flags;
	flags = get_pid(to) == pid::EMPTY ? Move::QUIETS : Move::CAPTURE;
	return Move(from, to, flags);
}

CB::Move CB::BoardRep::format_promo_mv(unsigned int to, unsigned int from, pid promo_piece)
{
	unsigned int flag;
	const unsigned int flag_arr[5] = { 0, Move::KNIGHT_PROMO,
		Move::BISHOP_PROMO, Move::ROOK_PROMO, Move::QUEEN_PROMO };
	flag = flag_arr[promo_piece];

	// Adding on 4 to the a promotion turns it into a capture promotion
	flag += get_pid(to) == EMPTY ? 0 : 4;
	return Move(from, to, flag);
}

std::vector<CB::Move> *CB::BoardRep::gen_move_list()
{
	std::vector<Move> *move_list;
	move_list = new std::vector<Move>;

	if (!tables_valid)
		update_all();

	append_simple_moves(move_list);
	append_castle_moves(move_list);
	append_enp_moves(move_list);
	append_dpawn_push(move_list);
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
			move_list->push_back(format_simple_mv(target, sq));
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

bool CB::BoardRep::ksc_legal() const
{
	if (!state_history.back().get_ksc_right(white_turn))
		return false;

	if ((white_turn ? W_KSC_OCC_MASK : B_KSC_OCC_MASK) &
			((bb.occupied ^ bb.piece[white_turn][KING]) | threats))
		return false;

	return true;
}

bool CB::BoardRep::qsc_legal() const
{
	if (!state_history.back().get_qsc_right(white_turn))
		return false;

	if ((white_turn ? W_QSC_OCC_MASK : B_QSC_OCC_MASK) &
			((bb.occupied ^ bb.piece[white_turn][KING]) | threats))
		return false;

	return true;
}

void CB::BoardRep::append_enp_moves(std::vector<Move> *move_list)
{
	if (!state_history.back().enp_availiable())
		return;

	int enp_sq = (white_turn ? LOWEST_B_ENP_ROW_SQ : LOWEST_W_ENP_ROW_SQ)
		+ state_history.back().get_enp_col();
	uint64_t enp_sources = Tables::read_patk(!white_turn, enp_sq) & bb.piece[white_turn][PAWN];
	while (enp_sources) {
		int sq = pop_rbit(enp_sources);
		CB::Move move = Move(sq, enp_sq, Move::ENPASSANT);

		// Make the potential move, then see if the king is in check, then back out
		make(move);
		uint64_t temp_checkers;
		int king_sq = peek_rbit(bb.piece[white_turn][KING]);
		temp_checkers = Tables::read_patk(white_turn, king_sq) & bb.piece[!white_turn][PAWN];
		temp_checkers |= Tables::read_natk(king_sq) & bb.piece[!white_turn][KNIGHT];
		temp_checkers |= Tables::read_batk(king_sq, bb.occupied)
			& (bb.piece[!white_turn][BISHOP] | bb.piece[!white_turn][QUEEN]);
		temp_checkers |= Tables::read_ratk(king_sq, bb.occupied)
			& (bb.piece[!white_turn][ROOK] | bb.piece[!white_turn][QUEEN]);
		unmake();

		if (temp_checkers)
			continue;

		move_list->push_back(move);
	}
}

void CB::BoardRep::append_dpawn_push(std::vector<Move> *move_list)
{
	uint64_t mask = white_turn ? BB_W_PAWN_HOME : BB_B_PAWN_HOME;

	uint64_t parser = mask & bb.piece[white_turn][PAWN];
	while (parser) {
		int from = pop_rbit(parser);
		int to = from + (white_turn ? -16 : 16);
		int passed = from + (white_turn ? -8 : 8);
		if (!(Tables::tf_table[to][passed] & bb.occupied)) {
			uint64_t legal_mask = get_pin_mask(from);
			legal_mask &= check_blocks;
			if (legal_mask & (1ULL << to))
				move_list->push_back(Move(from, to, Move::DOUBLE_PAWN_PUSH));
		}
	}
}

void CB::BoardRep::append_promos(std::vector<Move> *move_list)
{
	uint64_t pawns = bb.piece[white_turn][PAWN] & (white_turn ? BB_B_PAWN_HOME : BB_W_PAWN_HOME);
	while (pawns) {
		int direction = white_turn ? -1 : 1;
		int sq = pop_rbit(pawns);

		uint64_t legal_mask = get_pin_mask(sq);
		legal_mask &= check_blocks;

		int push_target = sq + 8 * direction;
		if (mailbox[push_target] == EMPTY && (1ULL << push_target) & legal_mask) {
			move_list->push_back(Move(push_target, sq, Move::KNIGHT_PROMO));
			move_list->push_back(Move(push_target, sq, Move::BISHOP_PROMO));
			move_list->push_back(Move(push_target, sq, Move::ROOK_PROMO));
			move_list->push_back(Move(push_target, sq, Move::QUEEN_PROMO));
		}

		uint64_t cap_targets = Tables::read_patk(white_turn, sq);
		cap_targets &= bb.color[!white_turn];
		while (cap_targets && cap_targets & legal_mask) {
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
		mask->special = get_pawn_sp_mask(sq);
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

uint64_t CB::BoardRep::get_pawn_sp_mask(int sq) const
{
	int row = sq / 8;
	int home_row = white_turn ? 6 : 1;
	int enp_row = white_turn ? 3 : 4;

	if (row == home_row) {
		int target = sq + (white_turn ? -16 : 16);
		int passed = sq + (white_turn ? -8 : 8);
		if (!(Tables::tf_table[target][passed] & bb.occupied))
			return (1ULL << target);
	} else if (row == enp_row && state_history.back().enp_availiable()) {
		int target = (row + (white_turn ? -1 : 1)) * 8 + state_history.back().get_enp_col();
		return (1ULL << target);
	}

	return 0;
}

uint64_t CB::BoardRep::get_castle_mask() const
{
	uint64_t moves = 0;
	if (ksc_legal()) {
		moves |= white_turn ? BB_W_KSC_TARGET : BB_B_KSC_TARGET;
	}

	if (qsc_legal()) {
		moves |= white_turn ? BB_W_QSC_TARGET : BB_B_KSC_TARGET;
	}

	return moves;
}

void CB::BoardRep::update_all()
{
	tables_valid = true;

	update_checks();
	update_check_blocks();
	update_pins();
}

void CB::BoardRep::update_checks()
{
	threats = pawn_smear(!white_turn);

	uint64_t pieces = bb.color[!white_turn] ^ bb.piece[!white_turn][PAWN];
	bb.occupied ^= bb.piece[white_turn][KING];
	while (pieces) {
		int sq = pop_rbit(pieces);
		// This is kind of hacky, I just remove the king from occ then put it back
		// to properly handle generating threats.
		threats |= get_pseudo_mv_mask(sq);
	}
	bb.occupied ^= bb.piece[white_turn][KING];

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

uint64_t CB::BoardRep::pawn_smear(bool is_white) const
{
	uint64_t output;

	if (is_white) {
		output = (bb.piece[true][PAWN] >> 9) & ~BB_R_COL; 
		output |= (bb.piece[true][PAWN] >> 7) & ~BB_L_COL;
		return output;
	}

	output = (bb.piece[false][PAWN] << 7) & ~BB_R_COL; 
	output |= (bb.piece[false][PAWN] << 9) & ~BB_L_COL;
	return output;
}

void CB::BoardRep::update_pins()
{
	pins[8] = 0;
	int king_sq = peek_rbit(bb.piece[white_turn][KING]);

	int i = 0;
	uint64_t pinner = xray_ratk(bb.occupied, bb.color[white_turn], king_sq)
		& (bb.piece[!white_turn][ROOK] | bb.piece[!white_turn][QUEEN]);
	while (pinner) {
		int sq = pop_rbit(pinner);
		pins[i] = Tables::tf_table[sq][king_sq];
		pins[8] ^= pins[i++];
	}

	pinner = xray_batk(bb.occupied, bb.color[white_turn], king_sq)
		& (bb.piece[!white_turn][BISHOP] | bb.piece[!white_turn][QUEEN]);
	while (pinner) {
		int sq = pop_rbit(pinner);
		pins[i] = Tables::tf_table[sq][king_sq];
		pins[8] ^= pins[i++];
	}

	pins[8] &= ~(1ULL << king_sq);
}

uint64_t CB::BoardRep::xray_ratk(uint64_t occ, uint64_t blockers, int sq) const
{
	uint64_t attacks = Tables::read_ratk(sq, occ);
	blockers &= attacks;
	return attacks ^ Tables::read_ratk(sq, occ ^ blockers);
}

uint64_t CB::BoardRep::xray_batk(uint64_t occ, uint64_t blockers, int sq) const
{
	uint64_t attacks = Tables::read_batk(sq, occ);
	blockers &= attacks;
	return attacks ^ Tables::read_batk(sq, occ ^ blockers);
}

inline int CB::BoardRep::get_pid(int sq) const
{
	return mailbox[sq];
}

uint64_t CB::BoardRep::get_pseudo_mv_mask(int sq) const
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

uint64_t CB::BoardRep::get_legal_mv_mask(int sq) const
{
	int pid = get_pid(sq);
	uint64_t moves = get_pseudo_mv_mask(sq);
	moves &= ~(bb.color[white_turn]);

	if (pid == KING) {
		king_mv_adjust(sq, moves);
		return moves;
	}

	uint64_t legal_mask = get_pin_mask(sq);
	legal_mask &= check_blocks;

	return moves & legal_mask;
}

uint64_t CB::BoardRep::pawn_moves(int sq) const
{
	uint64_t moves = Tables::read_patk(white_turn, sq) & bb.color[!white_turn];
	int target = sq + (white_turn ? -8 : 8);
	moves |= (1ULL << target) & ~bb.occupied;
	return moves;
}

void CB::BoardRep::king_mv_adjust(int sq, uint64_t &moves) const
{
	moves &= ~threats;
}

uint64_t CB::BoardRep::get_pin_mask(int sq) const
{
	if (!((1ULL << sq) & pins[8]))
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
	check_blocks = checkers | Tables::tf_table[king_sq][checkr_sq];
}

