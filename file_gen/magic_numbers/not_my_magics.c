#include <stdio.h>
#include <stdlib.h>

#define USE_32_BIT_MULTIPLICATIONS

// DEBUG
#define BYTE_TO_BINARY_PATTERN "%c %c %c %c %c %c %c %c \n\n"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


#define U64_TO_BB(u64)  \
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(u64)); \
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(u64 >> 8)); \
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(u64 >> 16)); \
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(u64 >> 24)); \
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(u64 >> 32)); \
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(u64 >> 40)); \
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(u64 >> 48)); \
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(u64 >> 56)); \
///////////////////////////////////////


typedef unsigned long long uint64_t;

// Gets a random uint64 by splicing together 4 uint16's.
uint64_t random_uint64() {
  uint64_t u1, u2, u3, u4;
  u1 = (uint64_t)(random()) & 0xFFFF; u2 = (uint64_t)(random()) & 0xFFFF;
  u3 = (uint64_t)(random()) & 0xFFFF; u4 = (uint64_t)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

// Reduces the number of set bits in the uint64 by finding the intersection
// of three random uint64's.
uint64_t random_uint64_fewbits() {
  return random_uint64() & random_uint64() & random_uint64();
}

// Cool bithack that is equivalent to the popcount.
// b &= b - 1 flips the first set bit.
int count_1s(uint64_t b) {
  int r;
  for(r = 0; b; r++, b &= b - 1);
  return r;
}

// A hash map for the pop_1st_bit function below.
const int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

// Equivalent to first_set_bit(), but also removes the bit in question.
// Takes in a uint64, then uses bit hacks to create a key which
// references the hash map above.
int pop_1st_bit(uint64_t *bb) {
  uint64_t b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
  *bb &= (*bb - 1);
  return BitTable[(fold * 0x783a9b23) >> 26];
}

// Returns an array of blockers given a square and a mask of occupancy bits.
uint64_t index_to_uint64(int index, int bits, uint64_t m) {
  // DEBUG
  printf("%d\n", index);
  printf("%d\n", bits);
  U64_TO_BB(m);
  printf("-----------------\n\n");
  ///////////////////////

  int i, j;
  uint64_t result = 0ULL;
  for(i = 0; i < bits; i++) {
    j = pop_1st_bit(&m);
    if(index & (1 << i)) result |= (1ULL << j);
  }

  //DEBUG
  U64_TO_BB(result);
  printf("==============================\n\n");
  //////////////
  return result;
}

// Generates the relavent occupancy bit masks for a particular
// square with a rook at that position.
uint64_t rmask(int sq) {
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
uint64_t bmask(int sq) {
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
uint64_t ratt(int sq, uint64_t block) {
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
uint64_t batt(int sq, uint64_t block) {
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

// The transformation hash function. Takes an input of the intersection
// of the relavent occupancy bits and the magic in question
// (Along with the minimum number of relavent bits).
// Then returns the potential key.
int transform(uint64_t b, uint64_t magic, int bits) {
#if defined(USE_32_BIT_MULTIPLICATIONS)
  return
    (unsigned)((int)b*(int)magic ^ (int)(b>>32)*(int)(magic>>32)) >> (32-bits);
#else
  return (int)((b * magic) >> (64 - bits));
#endif
}

// The cool boy function. Finds a magic at a given square.
uint64_t find_magic(int sq, int m, int bishop) {
  uint64_t mask, b[4096], a[4096], used[4096], magic;
  int i, j, k, n, fail;

  // Decide which mask to use, and figure out the number
  // of relavent bits in that mask.
  mask = bishop? bmask(sq) : rmask(sq);
  n = count_1s(mask);

  // Populates the arrays b[] and a[]. Fills b[] with all fo the possible 
  // occupancy squares, then fills a[] with the corresponding attack pattern.
  for(i = 0; i < (1 << n); i++) {
    b[i] = index_to_uint64(i, n, mask);
    a[i] = bishop? batt(sq, b[i]) : ratt(sq, b[i]);
  }
  for(k = 0; k < 100000000; k++) {
    // Get a potential magic.
    // Make sure it has 6 or more bits to save some time. Anything less couldn't be a valid magic.
    // Set all of used to 0, then resort a[] into used[] based
    // on the key made from the transformed magic.
    magic = random_uint64_fewbits();
    if(count_1s((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
    for(i = 0; i < 4096; i++) used[i] = 0ULL;
    for(i = 0, fail = 0; !fail && i < (1 << n); i++) {
      j = transform(b[i], magic, m);
      if(used[j] == 0ULL) used[j] = a[i];
      else if(used[j] != a[i]) fail = 1;
    }
    if(!fail) return magic;
  }
  printf("***Failed***\n");
  return 0ULL;
}

int RBits[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};

int BBits[64] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};

int main() {
  int square;

  printf("const uint64 RMagic[64] = {\n");
  for(square = 0; square < 64; square++)
    printf("  0x%llxULL,\n", find_magic(square, RBits[square], 0));
  printf("};\n\n");

  printf("const uint64 BMagic[64] = {\n");
  for(square = 0; square < 64; square++)
    printf("  0x%llxULL,\n", find_magic(square, BBits[square], 1));
  printf("};\n\n");

  return 0;
}
