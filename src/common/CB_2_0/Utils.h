#ifndef CB_2_0_UTILS
#define CB_2_0_UTILS

#include <cstdint>
#include <bit>

namespace CB
{

const uint64_t BB_R_COL = 0x8080808080808080;
const uint64_t BB_L_COL = 0x0101010101010101;
const uint64_t BB_TOP_ROW = 0x00000000000000FF;
const uint64_t BB_BOTTOM_ROW = 0xFF00000000000000; 
const uint64_t BB_FULL = 0xFFFFFFFFFFFFFFFF;
const uint64_t BB_B_PAWN_HOME = 0x000000000000FF00; // Second-to-top row
const uint64_t BB_W_PAWN_HOME = 0x00FF000000000000; // Second-to-bottom row

const int WKING_INIT_SQ = 60;
const int W_KSC_ROOK = 63;
const int W_QSC_ROOK = 56;
const int W_KSC_TARGET = 62;
const int W_QSC_TARGET = 58;
constexpr uint64_t BB_W_KSC_TARGET = 1ULL << W_KSC_TARGET;
constexpr uint64_t BB_W_QSC_TARGET = 1ULL << W_QSC_TARGET;
const uint64_t W_KSC_OCC_MASK = 0x6000000000000000;
const uint64_t W_QSC_OCC_MASK = 0x0E00000000000000;
const uint64_t W_KSC_CHK_MASK = 0x7000000000000000;
const uint64_t W_QSC_CHK_MASK = 0x1C00000000000000;

const int BKING_INIT_SQ = 4;
const int B_KSC_ROOK = 7;
const int B_QSC_ROOK = 0;
const int B_KSC_TARGET = 6;
const int B_QSC_TARGET = 2;
constexpr uint64_t BB_B_KSC_TARGET = 1ULL << B_KSC_TARGET;
constexpr uint64_t BB_B_QSC_TARGET = 1ULL << B_QSC_TARGET;
const uint64_t B_KSC_OCC_MASK = 0x0000000000000060;
const uint64_t B_QSC_OCC_MASK = 0x000000000000000E;
const uint64_t B_KSC_CHK_MASK = 0x0000000000000070;
const uint64_t B_QSC_CHK_MASK = 0x000000000000001C;

const int LOWEST_W_ENP_ROW_SQ = 40;
const int LOWEST_B_ENP_ROW_SQ = 16;


// PIECE ID: Includes color of piece in encoding
enum colored_pid {
	N_EMPTY = 0,
	B_KING = -6,
	B_QUEEN = -5,
	B_ROOK = -4,
	B_BISHOP = -3,
	B_KNIGHT = -2,
	B_PAWN = -1,
	W_PAWN = 1,
	W_KNIGHT = 2,
	W_BISHOP = 3,
	W_ROOK = 4,
	W_QUEEN = 5,
	W_KING = 6
};

// PIECE ID: Does not include color encoding
enum pid {
	PAWN = 0,
	KNIGHT = 1,
	BISHOP = 2,
	ROOK = 3,
	QUEEN = 4,
	KING = 5,
	EMPTY = 6
};

// A wrapper class containing extra data for positions that cannot survive 
// make-unmake. Stored in a stack with the 
//
// 4 bits for castling rights (KQkq)
// 1 bit for enp availiability or capture piece id
// 3 bits for enp column or captured piece id
// 6 bits for halfmove-clock (from 0 to 50)
class board_state_extra {
public:
	board_state_extra() { data = INIT_BS; }
	board_state_extra(const board_state_extra &old_bs) { data = old_bs.data; }

	bool get_qsc_right(bool is_white) const { return data & (0b100 >> (is_white * 2)); }
	bool get_ksc_right(bool is_white) const { return data & (0b1000 >> (is_white * 2)); }

	void remove_qsc_right(bool is_white) { data &= ~0b100 >> (is_white * 2); }
	void remove_ksc_right(bool is_white) { data &= ~0b1000 >> (is_white * 2); }
	void remove_castle_rights(bool is_white) { data &= ~0b1100 >> (is_white * 2); }

	void set_qsc_right(bool is_white) { data |= 0b100 >> (is_white * 2); }
	void set_ksc_right(bool is_white) { data |= 0b1000 >> (is_white * 2); }
	void set_castle_rights(bool is_white) { data |= 0b1100 >> (is_white * 2); }

	bool enp_availiable() const { return data & ENP_AVAILIABLE; }
	unsigned int get_enp_col() const { return (data & ENP_COL) >> 5; }
	unsigned int get_pid_col() const { return (data & PID_COL) >> 5; }
	
	void set_enp(unsigned int enp_col) {
		data = (data & ~ENP_COL) | (enp_col << 5);
		data |= ENP_AVAILIABLE;
	}
	void decay_enp() {
		data &= ~ENP_ALL;
	}
	void set_cap_piece(unsigned int cap_pice_id) {
		data = (data & ~PID_COL) | (cap_pice_id << 5);
		data &= ~ENP_AVAILIABLE;
	}

	unsigned int get_halfmove_clock() { return data & HALFMOVE_CLOCK; }
	void set_halfmove_clock(unsigned int clock) {
		data = (data & ~HALFMOVE_CLOCK) | (clock << 8);
	}

	~board_state_extra() = default;

private:
	uint16_t data;

	static const uint16_t INIT_BS = 0b0000;
	static const uint16_t ENP_COL = 0b11100000;
	static const uint16_t PID_COL = 0b11100000;
	static const uint16_t ENP_AVAILIABLE = 0b10000;
	static const uint16_t ENP_ALL = 0b11110000;
	static const uint16_t HALFMOVE_CLOCK = 0b11111100000000;
};

struct move_set {
	uint64_t push;
	uint64_t cap;
	uint64_t special;
	uint64_t promo;
};

inline int peek_rbit(const uint64_t &bb) { return std::__countr_zero(bb); }
inline int pop_rbit(uint64_t &bb) {
	int sq = std::__countr_zero(bb);
	bb ^= 1ULL << sq;
	return sq;
}
inline int popcnt(const uint64_t &bb) { return std::__popcount(bb); }

}

#endif
