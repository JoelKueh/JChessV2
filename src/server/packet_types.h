#include "../common/CB_2_0/Move.h"

#ifndef H_PACKET_TYPES
#define H_PACKET_TYPES

enum packet_type {
	move = 'M',
	request = 'R',
	join = 'J',
};

enum request_type {
	state = 'S'
};

struct move_body {
	CB::Move move;
};

struct request_body {
	enum request_type type;
};

struct join_body {
	char game_id[5];
};

union packet_body {
	struct move_body;
	struct board_state_body;
};

struct packet {
	char header;
	union packet_body body;
};

#endif
