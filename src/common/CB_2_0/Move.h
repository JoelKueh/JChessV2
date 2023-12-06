#ifndef CB_2_0_MOVE
#define CB_2_0_MOVE

#include <cstdint>
#include <string>

#include "Utils.h"

namespace CB 
{

class Move
{
public:
	Move() = default;

	Move(unsigned int from, unsigned int to, unsigned int flags);
	Move(uint16_t data);

	void operator=(const Move &rhs);

	unsigned int get_to() const { return data & TO; } 
	unsigned int get_from() const { return (data & FROM) >> 6; }
	unsigned int get_flags() const { return (data & FLAGS) >> 12; }
	bool is_invalid() const { return data == INVALID; }

	void set_to(unsigned int to) {
		data = (data & ~TO) | (to & 0x3f);
	}
	void set_from(unsigned int from) {
		data = (data & ~FROM) | ((from & 0x3f) << 6);
	}
	void set_flags(unsigned int flags) {
		data = (data & ~FLAGS) | ((flags & 0xf) << 12);
	}

	enum {
		QUIETS = 0,
		DOUBLE_PAWN_PUSH = 1,
		KING_SIDE_CASTLE = 2,
		QUEEN_SIDE_CASTLE = 3,
		CAPTURE = 4,
		ENPASSANT = 5,
		KNIGHT_PROMO = 8,
		BISHOP_PROMO = 9,
		ROOK_PROMO = 10,
		QUEEN_PROMO = 11,
		KNIGHT_PROMO_CAPTURE = 12,
		BISHOP_PROMO_CAPTURE = 13,
		ROOK_PROMO_CAPTURE = 14,
		QUEEN_PROMO_CAPTURE = 15 
	};
	static const uint16_t INVALID = 0b0110111111111111;

	std::string to_long_algbr();

protected:
	uint16_t data;

	static const uint16_t FLAGS = 0xf << 12;
	static const uint16_t FROM = 0x3f << 6;
	static const uint16_t TO = 0x3f;
};

}

#endif
