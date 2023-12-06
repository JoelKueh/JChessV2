
#ifndef H_SERVER
#define H_SERVER

#define DEBUG_VERBOSE

#include "../../common/CB_2_0/BoardRep.h"

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <unordered_map>

#define PORT 24388
#define MAX_CLIENTS 30
#define MAX_GAMES 15

#define MAX_EVENTS 5
#define READ_SIZE 100
#define MAX_COMMAND_LENGTH 100


class server
{
public:
	server();
	~server();

	/**
	 * Wait for linux to send a signal on the epoll set that there is
	 * data on one of the file descriptors in the set. The master fd
	 * handles connections, fd:0 handles stdio, and each client has it's
	 * own fd that handles reads, writes, and disconnects.
	 */
	int poll_set();

private:
	struct epoll_entry;
	struct client;
	struct game;

	enum command_type {
		MOVE,
		JOIN,
		CREATE_ROOM,
		UPDATE,
		RESIGN,
		EXIT,
		INVALID
	};

	struct epoll_entry {
		int fd;
		struct epoll_event event;
	};

	struct game
	{
		CB::BoardRep *board;
		int times[2];
		bool is_timed;
		int incr;

		struct client *clients[2];
		std::time_t go_time; // Time at which the the current client
				     // was sent to search for a move.
		
		char game_id[5];
	};

	struct client
	{
		struct epoll_event event;
		struct game *game;
		char command[MAX_COMMAND_LENGTH + 1];
		int comlen;
		bool is_white;
		bool awaiting_move;
	};

	int epoll_fd;
	struct epoll_entry e_stdin;
	struct epoll_entry e_master;
	std::unordered_map<std::string, struct game*> games;
	std::unordered_map<int, struct client*> clients;
	
	int handle_events(struct epoll_event *event);
	int handle_data(struct epoll_event *event);
	int handle_disconnect(struct epoll_event *event);
	void handle_master();
	void handle_client(struct client *cli, char read_buffer[]);

	void parse_command(struct client *cli);
	enum command_type parse_head(char head[]);

	/**
	 * This command is sent whenever the client wants to move a piece.
	 */
	void parse_move(struct client *cli, char **words, int num_words);
	/**
	 * This command is sent whenever a client wants to start a new game
	 */
	void parse_create(struct client *cli, char **words, int num_words);
	/**
	 * This command is sent whenever a client wants to join an already
	 * existing game
	 */
	void parse_join(struct client *cli, char **words, int num_words);
	/**
	 * This command is sent whenever a client has lost the state of the
	 * game and needs to play the game again from root.
	 */
	void parse_update(struct client *cli, char **words, int num_words);
	/**
	 * This command is sent whenever a client has lost the state of the
	 * game and needs to play the game again from root.
	 */
	void parse_resign(struct client *cli, char **words, int num_words);
	/**
	 * This command should be sent when a client wants to disconnect
	 * gracefully. Otherwise it will be killed upon the next write.
	 */
	void parse_exit(struct client *cli, char **words, int num_words);
	void send_state(struct client *cli);

	void send_client(struct client *cli, const char msg[]);
	void send_client(struct client *cli, const char msg[], int len);
	void close_game(struct game *game);
	void close_client(struct client *cli);
};

#endif
