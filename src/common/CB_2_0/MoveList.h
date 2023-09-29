#ifndef MOVE_LIST
#define MOVE_LIST

#include "Move.h"

namespace CB {

class MoveList {
public:
	MoveList() = default;

	inline int size() { return length; }
	inline void push_back(CB::Move move) { data[length++] = move; }
	inline CB::Move at(int i) { return data[i]; }

	~MoveList() = default;
private:
	int length = 0;
	CB::Move data[218];
};

}

#endif
