#pragma once

#include <cstdint>

namespace pieces
{
	enum piece {
		NONE   = 0,
		PAWN_E = 1,
		PAWN   = 2,
		KNIGHT = 3,
		BISHOP = 4,
		ROOK   = 5,
		QUEEN  = 6,
		KING   = 7,
	};

	const short WHITE = 1;
	const short BLACK = -1;
}

typedef uint32_t UChar;
const UChar hz_ln = 0x2500;
const UChar vr_ln = 0x2502;
const UChar d_r = 0x250c;
const UChar d_lr = 0x252c;
const UChar d_l = 0x2510;
const UChar du_r = 0x251c;
const UChar du_lr = 0x253c;
const UChar du_l = 0x2524;
const UChar u_r = 0x2514;
const UChar u_lr = 0x2534;
const UChar u_l = 0x2518;
const UChar r_half = 0x2590;
const UChar l_half = 0x258c;

const wchar_t r_half_str[] = { r_half, '\0' };
const wchar_t l_half_str[] = { l_half, '\0' };

struct options
{
	bool uni = true;
	bool gui = false;
};
