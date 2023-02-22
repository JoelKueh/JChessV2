#include "magics.h"

// A hash map for the pop_1st_bit function below.
const int chessboard::tables::BitTable[64] = {
	63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
	51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
	26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
	58, 20, 37, 17, 36, 8
};

// Equivalent to first_set_bit(), but also removes the bit in question.
// Takes in a uint64, then uses bit hacks to create a key which
// references the hash map above.
int chessboard::tables::pop_1st_bit(uint64_t *bb) {
	uint64_t b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	return BitTable[(fold * 0x783a9b23) >> 26];
}

// Returns an array of blockers given a square and a mask of occupancy bits.
uint64_t chessboard::tables::index_to_uint64(int index, int bits, uint64_t m) {
	int i, j;
	uint64_t result = 0ULL;
	for(i = 0; i < bits; i++) {
		j = pop_1st_bit(&m);
		if(index & (1 << i)) result |= (1ULL << j);
	}
	return result;
}

int chessboard::tables::transform(uint64_t board, int magic, int bits) {
	return (board * magic) >> (64 - bits);
}

// Generates the relavent occupancy bit masks for a particular
// square with a rook at that position.
uint64_t chessboard::tables::rmask(int sq) {
	uint64_t result = 0ULL;
	int rk = sq/8, fl = sq%8, r, f;
	for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
	for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
	for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
	for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
	return result;
}

// Generates the relavent occupancy bit masks for a particular
// square with a bishop at that position.
uint64_t chessboard::tables::bmask(int sq) {
	uint64_t result = 0ULL;
	int rk = sq/8, fl = sq%8, r, f;
	for(r=rk+1, f=fl+1; r<=6 && f<=6; r++, f++) result |= (1ULL << (f + r*8));
	for(r=rk+1, f=fl-1; r<=6 && f>=1; r++, f--) result |= (1ULL << (f + r*8));
	for(r=rk-1, f=fl+1; r>=1 && f<=6; r--, f++) result |= (1ULL << (f + r*8));
	for(r=rk-1, f=fl-1; r>=1 && f>=1; r--, f--) result |= (1ULL << (f + r*8));
	return result;
}

// Generates the corresponding attack squares for the rook at a given position.
// Breaks out if there is a blocker. This will be used to fill the hash table.
uint64_t chessboard::tables::ratt(int sq, uint64_t block) {
	uint64_t result = 0ULL;
	int rk = sq/8, fl = sq%8, r, f;
	for(r = rk+1; r <= 7; r++) {
		result |= (1ULL << (fl + r*8));
		if(block & (1ULL << (fl + r*8))) break;
	}
	for(r = rk-1; r >= 0; r--) {
		result |= (1ULL << (fl + r*8));
		if(block & (1ULL << (fl + r*8))) break;
	}
	for(f = fl+1; f <= 7; f++) {
		result |= (1ULL << (f + rk*8));
		if(block & (1ULL << (f + rk*8))) break;
	}
	for(f = fl-1; f >= 0; f--) {
		result |= (1ULL << (f + rk*8));
		if(block & (1ULL << (f + rk*8))) break;
	}
	return result;
}

// Generates the corresponding attack squares for a bishop at a given position.
// Breaks out if there is a blocker. This will be used to fill the hash table.
uint64_t chessboard::tables::batt(int sq, uint64_t block) {
	uint64_t result = 0ULL;
	int rk = sq/8, fl = sq%8, r, f;
	for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) {
		result |= (1ULL << (f + r*8));
		if(block & (1ULL << (f + r * 8))) break;
	}
	for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) {
		result |= (1ULL << (f + r*8));
		if(block & (1ULL << (f + r * 8))) break;
	}
	for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) {
		result |= (1ULL << (f + r*8));
		if(block & (1ULL << (f + r * 8))) break;
	}
	for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) {
		result |= (1ULL << (f + r*8));
		if(block & (1ULL << (f + r * 8))) break;
	}
	return result;
}

int chessboard::tables::RBits[64] = {
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12
};

int chessboard::tables::BBits[64] = {
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6
};

int *chessboard::tables::RAtkTable[64];
int *chessboard::tables::BAtkTable[64];

void chessboard::tables::hashtab(bool bishop) {
	uint64_t legal_moves[4096], occupied[4096];
	int i, j, k;

	for (i = 0; i < 64; ++i) {
		uint64_t mask = bishop ? bmask(i) : rmask(i);
		int *atk_table = bishop ? BAtkTable[i] : RAtkTable[i];
		int bits = bishop ? BBits[i] : RBits[i];
		uint64_t magic = bishop ? BMagic[i] : RMagic[i];

		atk_table = new int[(1 << bits)];
		
		for (j = 0; j < (1 << bits); ++j) {
			occupied[j] = index_to_uint64(j, bits, mask);
			legal_moves[j] = bishop ? batt(i, occupied[j]) : ratt(i, occupied[j]);
		}
		for (j = 0; j < (1 << bits); ++j) {
			k = transform(occupied[j], magic, bits);
			atk_table[j] = legal_moves[k];
		}
	}
}

void chessboard::tables::hashtab()
{
	hashtab(true);
	hashtab(false);
}