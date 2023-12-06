
#include "Move.h"

CB::Move::Move(unsigned int from, unsigned int to, unsigned int flags) {
	data = ((flags & 0xf) << 12)
		| ((from & 0x3f) << 6)
		| (to & 0x3f);
}

CB::Move::Move(uint16_t data) {
	this->data = data;
}

void CB::Move::operator=(const Move &rhs) {
	this->data = rhs.data;
}

std::string CB::Move::to_long_algbr()
{
	std::string algbr;
	algbr.push_back(get_from() % 8 + 'a');
	algbr.push_back('8' - get_from() / 8);
	algbr.push_back(get_to() % 8 + 'a');
	algbr.push_back('8' - get_to() / 8);

	switch (get_flags())
	{
		case CB::Move::KNIGHT_PROMO:
		case CB::Move::KNIGHT_PROMO_CAPTURE:
			algbr.push_back('n');
			break;
		case CB::Move::BISHOP_PROMO:
		case CB::Move::BISHOP_PROMO_CAPTURE:
			algbr.push_back('b');
			break;
		case CB::Move::ROOK_PROMO:
		case CB::Move::ROOK_PROMO_CAPTURE:
			algbr.push_back('r');
			break;
		case CB::Move::QUEEN_PROMO:
		case CB::Move::QUEEN_PROMO_CAPTURE:
			algbr.push_back('q');
			break;
		default:
			break;
	}

	return algbr;
}
