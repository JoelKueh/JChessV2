
#include "scene.h"

#ifdef FILE_DEBUG
#include <fstream>
extern std::ofstream debug_out;
#endif

scene::scene() = default;
scene::~scene()
{
#if defined(FILE_DEBUG) && defined(DEBUG_VERBOSE)
	debug_out << "Destroying Base Scene" << std::endl;
#endif
}
