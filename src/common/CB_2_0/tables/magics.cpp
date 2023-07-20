#include <cstdint>
#include "magics.h"

#include <bit>

// Equivalent to first_set_bit(), but also removes the bit in question.
// Takes in a uint64, then uses bit hacks to create a key which
// references the hash map above.
int CB::Tables::pop_1st_bit(uint64_t *bb) {
	uint64_t b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	return BitTable[(fold * 0x783a9b23) >> 26];
}

// Returns an array of blockers given a square and a mask of occupancy bits.
uint64_t CB::Tables::index_to_uint64(int index, int bits, uint64_t m) {
	int i, j;
	uint64_t result = 0ULL;
	for(i = 0; i < bits; i++) {
		j = pop_1st_bit(&m);
		if(index & (1 << i)) result |= (1ULL << j);
	}
	return result;
}

int CB::Tables::transform(uint64_t board, uint64_t magic, int bits) {
	return (board * magic) >> (64 - bits);
}

// Generates the relavent occupancy bit masks for a particular
// square with a rook at that position.
uint64_t CB::Tables::rmask(int sq) {
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
uint64_t CB::Tables::bmask(int sq) {
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
uint64_t CB::Tables::ratt(int sq, uint64_t block) {
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
uint64_t CB::Tables::batt(int sq, uint64_t block) {
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

void CB::Tables::hashtab(bool bishop) {
	uint64_t legal_moves[4096], occupied[4096];
	int i, j, k;

	for (i = 0; i < 64; ++i) {
		uint64_t mask = bishop ? bmask(i) : rmask(i);
		uint64_t **atk_table = bishop ? &BAtkTable[i] : &RAtkTable[i];
		int bits = bishop ? BBits[i] : RBits[i];
		uint64_t magic = bishop ? BMagic[i] : RMagic[i];
		uint64_t *occupancy = bishop ? &BOccupancy[i] : &ROccupancy[i];
		*occupancy = mask;

		*atk_table = new uint64_t[(1 << bits)];
		
		for (j = 0; j < (1 << bits); ++j) {
			occupied[j] = index_to_uint64(j, bits, mask);
			legal_moves[j] = bishop ? batt(i, occupied[j]) : ratt(i, occupied[j]);

			(*atk_table)[j] = 0;
		}

		for (j = 0; j < (1 << bits); ++j) {

			k = transform(occupied[j], magic, bits);

			(*atk_table)[k] = legal_moves[j];
		}
	}
}

void CB::Tables::hashtab()
{
	hashtab(true);
	hashtab(false);
}

uint64_t CB::Tables::read_ratk(int sq, uint64_t board)
{
	board &= ROccupancy[sq];
	int index = transform(board, RMagic[sq], RBits[sq]);

	return RAtkTable[sq][index];
}

uint64_t CB::Tables::read_batk(int sq, uint64_t board)
{
	board &= BOccupancy[sq];
	int index = transform(board, BMagic[sq], BBits[sq]);
	return BAtkTable[sq][index];
}
