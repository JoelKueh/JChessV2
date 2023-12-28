
#ifndef H_CLIENT
#define H_CLIENT

#include "../common/CB_2_0/BoardRep.h"
#include "defs.h"
#include "game_cli.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <ncurses.h>

class client
{
private:

	struct server 
	{
		struct epoll_event event;
		char command[MAX_COMMAND_LENGTH + 1];
		int comlen;
	};

	enum mesasge_type {
		MOVE,
		RESIGN,
		GO,
		FEN_MAIN,
		FEN_MOVE,
		FEN_END,
		INVALID
	};

	bool reading_fen;
	bool awaiting_move;
	int epoll_fd;
	struct epoll_event e_stdin;
	struct epoll_event e_server;
	game_cli cli;
	int move_buf[4];

	int handle_events(struct epoll_event *event);
	int handle_data(struct epoll_event *event);
	int handle_disconnect(struct epoll_event *event);

	int handle_ncurses(char *rdbuf, int bytes);
	int ncurses_char(char input);
	void handle_server(char *rdbuf);

	void parse_move();
	void parse_fen();

	void spawn_server();
	void join_server(std::string ip);
	void create_game();
	void join_game();

	void send_move(CB::Move move);
	void send_resign();
	void send_exit();

	void close_server();
	void send_server(std::string msg);

public:
	client();
	int poll_set();
	~client();
};

#endif
