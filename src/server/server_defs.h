
#ifndef H_SERVER_DEFS
#define H_SERVER_DEFS

#include "../common/CB_2_0/BoardRep.h"

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <chrono>
#include <thread>

#define PORT 24388
#define MAX_CLIENTS 30
#define MAX_GAMES 15

#define MAX_EVENTS 5
#define READ_SIZE 10
#define MAX_COMMAND_LENGTH 100

typedef struct epoll_entry epoll_entry;
typedef struct client client;
typedef struct server server;
typedef struct game game;

enum command_type {
	MOVE,
	JOIN,
	CREATE_ROOM,
	UPDATE,
	INVALID
};

struct epoll_entry {
	int fd;
	struct epoll_event event;
};

// Board should be deleted and game_id should be freed when game is destroyed.
struct game
{
	CB::BoardRep *board;
	bool is_timed;
	long incr;

	client *clients[2] = { nullptr };
	long times[2];
	std::time_t go_time; // The time at which the the current client
			     // was sent to search for a move.

	char game_id[5] = "";
};

struct client {
	struct epoll_entry entry;
	struct game *game;
	char command[MAX_COMMAND_LENGTH + 1];
	int comlen;
	bool is_white;
	bool awaiting_move;
};

struct server {
	int epoll_fd;
	epoll_entry stdin;
	epoll_entry master;
	struct client clients[MAX_CLIENTS];
	struct game *games[MAX_GAMES];
	int num_games = 0;
};

#endif /* H_SERVER_DEFS */
