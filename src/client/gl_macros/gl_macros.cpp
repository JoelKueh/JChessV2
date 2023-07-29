#include "gl_macros.h"

void read_file(const char* fname, std::string &output)
{
	std::ifstream in(fname);

	// DEBUG
	if (!in.is_open())
		std::cout << "Failed to open " << fname << std::endl;

	output = std::string(std::istreambuf_iterator<char>(in),
				std::istreambuf_iterator<char>());
	in.close();
}
