#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>

std::ofstream tf_table;

uint64_t to_from(int to, int from)
{
	uint64_t manip = 1;
	uint64_t output = 0;

	if (from > to)
		return to_from(from, to);

	manip <<= from;

	int to_x = to % 8;
	int to_y = to / 8;

	int from_x = from % 8;
	int from_y = from / 8;

	int offset = 0;
	if (to_x - from_x == to_y - from_y)
		offset = 9;
	else if (to_x - from_x == -(to_y - from_y))
		offset = 7;
	else if (to_x == from_x)
		offset = 8;
	else if (to_y == from_y)
		offset = 1;
	else
		return 0;

	int i = from;
	while (i <= to)
	{
		output ^= manip;
		i += offset;
		manip <<= offset;
	}

	return output;
}

void print_bitfield(uint64_t field)
{
	uint64_t manip = 1;
	int bit;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			bit = field & manip;
			std::cout << " " << bit;
			field >>= 1;
		}
		std::cout << "\n";
	}
}

int main()
{
	tf_table.open("../../chessboard/tf_table.h");
	tf_table << "#pragma once\n#include <cstdint>\n\n"
		<< "namespace chessboard {\n\n"
		<< "namespace fields {\n\n"
		<< "const uint64_t tf_table[64][64] = {\n";

	for (int i = 0; i < 64; i++) {
		tf_table << "{";
		for (int j = 0; j < 64; j++) {
			if (j % 8 == 0)
				tf_table << "\n   ";
			tf_table << "0x" << std::setfill('0') << std::setw(sizeof(uint64_t)*2) << std::hex << to_from(i, j) << "ULL, ";
		}
		tf_table << "},\n";
	}

	tf_table << "};\n\n}\n\n}";

	return 0;
}