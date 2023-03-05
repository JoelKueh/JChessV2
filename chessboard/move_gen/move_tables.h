#pragma once

#include <cstdint>
#include "magics.h"

namespace ChessBoard {

namespace MoveTables {

const uint64_t left_two_cols = 0xC0C0C0C0C0C0C0C0;
const uint64_t right_two_cols = 0x0303030303030303;
static uint64_t NAtkTable[64] = { 0 };
static uint64_t KAtkTable[64] = { 0 };

static bool tables_generated = false;

void gen_move_tables();
void gen_natk();
uint64_t read_natk(int sq);
void gen_katk();
uint64_t read_katk(int sq);
void hashtab();

}

}