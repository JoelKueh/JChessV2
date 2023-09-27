#ifndef MOVE_LIST
#define MOVE_LIST

#include "Move.h"

namespace CB {

class MoveList {
public:
	MoveList() = default;

	int size() { return length; }
	void push_back(CB::Move move) { data[length++] = move; }
	CB::Move at(int i) { return data[i]; }

	~MoveList() = default;
private:
	int length = 0;
	CB::Move data[218];
};

}

#endif
