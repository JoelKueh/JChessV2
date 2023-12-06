
#include <iostream>
#include <string.h>
#include <errno.h>
#include "server.h"
#include "net_utils.h"

server::server()
{
	// Epoll filedecriptor creation
	epoll_fd = epoll_create(MAX_CLIENTS + 2);
	if (epoll_fd == -1) {
		std::cerr << "Epoll Create: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Master Socket Initilization
	if (!(e_master.fd = socket(AF_INET, SOCK_STREAM, 0))) {
		std::cerr << "Master Socket Create: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	int opt = 1;
	int addrlen = sizeof(addr);
	if (setsockopt(e_master.fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
				sizeof(opt)) < 0) {
		std::cerr << "Master Socket Option: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (bind(e_master.fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		std::cerr << "Master Socket Bind: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(e_master.fd, 3) < 0) {
		std::cerr << "Master Socket Listen: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Adding stdin to the epoll_fd
	e_stdin.fd = 0;
	e_stdin.event.events = EPOLLIN;
	e_stdin.event.data.fd = e_stdin.fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e_stdin.fd,
				&e_stdin.event)) {
		std::cerr << "STDIN Entry Create: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Adding the master socket to the epoll_fd
	e_master.event.events = EPOLLIN;
	e_master.event.data.fd = e_master.fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e_master.fd,
				&e_master.event)) {
		std::cerr << "Master Entry Create: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

int server::poll_set()
{
	int event_count;
	struct epoll_event events[MAX_EVENTS];

	// Polling for input...
	event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
	if (event_count == -1) {
		std::cerr << "Epoll Wait: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Cycling through ready events
	for (int i = 0; i < event_count; ++i) {
		if (handle_events(&events[i]) == 0)
			return 0;
	}
	return 1;
}

int server::handle_events(struct epoll_event *event)
{
	if (event->events & EPOLLRDHUP) {
#ifdef DEBUG_VERBOSE
		std::cout << "CLIENT DISCONNECTED" << std::endl;
#endif
		int status = handle_disconnect(event);
		return status;
	}

	if (event->events & EPOLLIN) {
		int status = handle_data(event);
		return status;
	}

	std::cout << "Unexpected Event" << std::endl;
	return 0;
}

int server::handle_data(struct epoll_event *event)
{
	// First check if we are handling an event on the master.
	if (event->data.fd == e_master.fd) {
		handle_master();
		return 1;
	}

	int bytes;
	char read_buffer[READ_SIZE + 1];
	bytes = read(event->data.fd, read_buffer, READ_SIZE);
	if (bytes == -1) {
		std::cerr << "Read: " << strerror(errno) << std::endl;
		return 0;
	}
	read_buffer[bytes] = '\0';

#ifdef DEBUG_VERBOSE
	std::cout << bytes << " bytes from client with fd " << event->data.fd
		<< ": " << read_buffer << std::endl; 
#endif

	if (event->data.fd == 0) {
		if (strcmp(read_buffer, "stop\n") == 0)
			return 0;
		else
			return 1;
	}

	struct client *cli = clients[event->data.fd];
	handle_client(cli, read_buffer);
	return 1;
}

int server::handle_disconnect(struct epoll_event *event)
{
	// We will never check for disconnects on master or stdin.
	// So we can assume this is a client.
	client *cli = clients[event->data.fd];

	bool is_white = cli->is_white;
	if (cli->game != nullptr && cli->game->clients[!is_white] != nullptr) {
		std::string msg = "resign|";
		msg.append(is_white ? "white" : "black");
		msg.push_back('\n');
		send_client(cli->game->clients[!is_white],
				msg.c_str(), msg.size());
		close_game(cli->game);
	}

	close_client(cli);
	return 1;
}

void server::handle_master()
{
#ifdef DEBUG_VERBOSE
	std::cout << "Client attempted connection." << std::endl;
#endif

	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	int new_socket = accept(e_master.fd, (struct sockaddr *)&addr,
			(socklen_t*)&addrlen);
	
	if (new_socket == -1) {
		std::cerr << "Accept: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (clients.size() == MAX_CLIENTS) {
		std::cout << "Too many clients" << std::endl;
		close(new_socket);
		return;
	}

	// Create the new client and pass it into the unordered_map
	struct client *cli = new client;
	cli->event.data.fd = new_socket;
	cli->event.events = EPOLLIN | EPOLLRDHUP;
	cli->game = nullptr;
	cli->command[0] = '\0';
	cli->comlen = 0;
	cli->is_white = false;
	cli->awaiting_move = false;
	clients[new_socket] = cli;

#ifdef DEBUG_VERBOSE
	std::cout << "Client File Descriptor: " << new_socket << std::endl;
#endif

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket,
				&cli->event)) {
		std::cerr << "Epoll Socket Add: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void server::handle_client(struct client *cli, char rb[])
{
	const char invalid_msg[] = "invalid_command\n";

	/**
	 * Parse the read buffer into the command buffer,
	 * executing the command when '\n' is encountered and terminating
	 * if '\0' is encountered as this causes undefined behavior in input.
	 */
	int i = 0;
	char ch;
	while ((ch = rb[i++])) {
		if (ch == '\n') {
			cli->command[cli->comlen++] = '\0';
			parse_command(cli);
		} else if (ch == '\0') {
			send_client(cli, invalid_msg);
		} else {
			if (cli->comlen == MAX_COMMAND_LENGTH) {
				send_client(cli, invalid_msg);
				cli->comlen = 0;
			}
			cli->command[cli->comlen++] = ch;
		}
	}
}

void server::parse_command(struct client *cli)
{
	const char invalid_msg[] = "invalid_command\n";

	char **words;
	int num_words, i;
	num_words = split_command(cli->command, &words);

#ifdef DEBUG_VERBOSE
	std::cout << "Client File Descriptor: " << cli->event.data.fd
		<< "\nCommand: ";
	for (int i = 0; i < num_words; ++i) {
		std::cout << words[i] << ' ';
	}
	std::cout << std::endl << std::endl;
#endif

	enum command_type type = parse_head(words[0]);
	
	switch (type)
	{
	case command_type::MOVE:
		parse_move(cli, words,  num_words);
		break;
	case command_type::JOIN:
		parse_join(cli, words, num_words);
		break;
	case command_type::CREATE_ROOM:
		parse_create(cli, words, num_words);
		break;
	case command_type::UPDATE:
		parse_update(cli, words, num_words);
		break;
	case command_type::RESIGN:
		parse_resign(cli, words, num_words);
		break;
	case command_type::EXIT:
		parse_exit(cli, words, num_words);
		break;
	default:
		send_client(cli, invalid_msg);
		break;
	}
	cli->comlen = 0;
	delete words;
}

enum server::command_type server::parse_head(char *head)
{
	const char move[] = "move";
	const char join[] = "join";
	const char create_room[] = "create_room";
	const char update[] = "update";
	const char resign[] = "resign";
	const char exit[] = "exit";

	if (strcmp(head, move) == 0) {
		return command_type::MOVE;
	} else if (strcmp(head, join) == 0) {
		return command_type::JOIN;
	} else if (strcmp(head, create_room) == 0) {
		return command_type::CREATE_ROOM;
	} else if (strcmp(head, update) == 0) {
		return command_type::UPDATE;
	} else if (strcmp(head, resign) == 0) {
		return command_type::RESIGN;
	} else if (strcmp(head, exit) == 0) {
		return command_type::EXIT;
	} else {
		return command_type::INVALID;
	}
}

void server::parse_move(struct client *cli, char **words, int num_words)
{
	const char invalid_msg[] = "invalid_command\n";
	const char invalid_move[] = "invalid_move\n";
	const char invalid_game[] = "invalid_no_game\n";
	const char invalid_go[] = "invalid_no_go\n";
	const char go[] = "go\n";

	struct game *game = cli->game;
	if (game == nullptr) {
		send_client(cli, invalid_game);
		return;
	}

	if (num_words != 2) {
		send_client(cli, invalid_msg);
		return;
	}

	if (game->board->is_white_turn() != cli->is_white) {
		send_client(cli, invalid_go);
		return;
	}

	std::string algbr = words[1];
	CB::Move move = game->board->algbr_to_move(algbr);

	if (move.is_invalid()) {
		send_client(cli, invalid_move);
		return;
	}

	// Update board and check state.
	std::string msg = "moved|";
	game->board->make(move);
	msg.append(algbr + "|");
	msg.append(cli->is_white ? "white|" : "black|");

	switch (game->board->get_board_state()) {
	case CB::BoardRep::bstate::NORMAL:
		msg.append("normal|");
		break;
	case CB::BoardRep::bstate::CHECK:
		msg.append("check|");
		break;
	case CB::BoardRep::bstate::STALEMATE:
		msg.append("stalemate|");
		break;
	case CB::BoardRep::bstate::WIN_BLACK:
		msg.append("win_black|");
		break;
	case CB::BoardRep::bstate::WIN_WHITE:
		msg.append("win_white|");
		break;
	default:
		break;
	}

	msg.append(std::to_string(game->times[0]));
	msg.push_back('|');
	msg.append(std::to_string(game->times[1]));
	msg.push_back('\n');

	send_client(game->clients[0], msg.c_str(), msg.length());
	send_client(game->clients[1], msg.c_str(), msg.length());
	bool is_white_turn = game->board->is_white_turn();
	send_client(game->clients[is_white_turn], go);

#ifdef DEBUG_VERBOSE
	std::cout << "\nGameID: " << cli->game->game_id << '\n';
	std::cout << "- Time: " << cli->game->times[0] << '\n';
	std::cout << "- Incr: " << cli->game->incr << '\n';
	print_board(*cli->game->board);
#endif
}

void server::parse_create(struct client *cli, char **words, int num_words)
{
	const char invalid_game_id[] = "invalid_game_id\n";
	const char invalid_in_game[] = "invalid_in_game\n";
	const char invalid_msg[] = "invalid_msg\n";
	const char white[] = "white";

	// Return if the client is already in a game
	if (cli->game != nullptr) {
		send_client(cli, invalid_in_game);
		return;
	}

	// Prevent out of bounds access on words.
	if (num_words != 5) {
		send_client(cli, invalid_msg);
		return;
	}

	char *game_id = words[1];
	char *color = words[2];
	char *time = words[3];
	char *incr = words[4];

	// Check if the length of the game_id is 4.
	// This works because **words is a series of
	// positions in client->command
	int game_id_len = color - game_id - 1;
	if (game_id_len != 4) {
		send_client(cli, invalid_game_id);
		return;
	}

	// Update the color of the client
	cli->is_white = (strcmp(color, white) == 0);

	// Inputs are validated, proceed with creating the new game.
	struct game *new_game = new game;
	new_game->board = new CB::BoardRep; // HEAP ALLOCATED
	new_game->times[0] = strtol(time, nullptr, 10) * 1000;
	new_game->times[1] = new_game->times[0];
	new_game->is_timed = (new_game->times[0] != 0);
	new_game->incr = strtol(incr, nullptr, 10) * 1000;

	new_game->clients[cli->is_white] = cli;
	new_game->clients[!(cli->is_white)] = nullptr;
	memcpy(new_game->game_id, game_id, 5);

	// Give the game a place in the unordered_map and update client
	games[new_game->game_id] = new_game;
	cli->game = new_game;
	cli->awaiting_move = false;

	send_state(cli);

#ifdef DEBUG_VERBOSE
	std::cout << "\nGameID: " << cli->game->game_id << '\n';
	std::cout << "- Time: " << cli->game->times[0] << '\n';
	std::cout << "- Incr: " << cli->game->incr << '\n';
	print_board(*cli->game->board);
#endif
}

void server::parse_join(struct client *cli, char **words, int num_words)
{
	const char invalid_game_id[] = "invalid_game_id";
	const char game_full[] = "invalid_game_already_full";
	const char invalid_in_game[] = "invalid_in_game";
	const char invalid_msg[] = "invalid_msg";
	int i;

	// Prevent the client from joining a new game if it is already in one.
	if (cli->game != nullptr) {
		send_client(cli, invalid_in_game);
		return;
	}

	// Prevent out of bounds access on words.
	if (num_words != 2) {
		send_client(cli, invalid_msg);
		return;
	}
	
	char *game_id = words[1];
	auto game_iter = games.find(game_id);
	
	if (game_iter == games.end()) {
		send_client(cli, invalid_game_id);
		return;
	}

	game *this_game = game_iter->second;
	if (this_game->clients[0] == nullptr
			|| this_game->clients[0] == cli) {
		this_game->clients[0] = cli;
		cli->is_white = false;
	} else if (this_game->clients[1] == nullptr
			|| this_game->clients[1] == cli) {
		this_game->clients[1] = cli;
		cli->is_white = true;
	} else {
		send_client(cli, game_full);
		return;
	}
	cli->game = this_game;
	cli->awaiting_move = false;

	send_state(cli);

#ifdef DEBUG_VERBOSE
	std::cout << "\nGameID: " << cli->game->game_id << '\n';
	std::cout << "- Time: " << cli->game->times[0] << '\n';
	std::cout << "- Incr: " << cli->game->incr << '\n';
	print_board(*cli->game->board);
#endif
}

void server::send_state(struct client *cli)
{
	std::string msg = "fen|";
	std::vector<std::string> fen = cli->game->board->fen_from_root();
	msg.append(fen[0]);
	msg.push_back('\n');

	for (int i = 1; i < fen.size(); ++i) {
		msg.append(fen[i]);
		msg.push_back('\n');
	}

	msg.append("end_fen\n");
	send_client(cli, msg.c_str(), msg.size());
}

void server::parse_update(struct client *cli, char **words, int num_words)
{
	send_state(cli);
}

void server::parse_resign(struct client *cli, char **words, int num_words)
{
	const char invalid_no_game[] = "invalid_no_game";

	if (cli->game == nullptr) {
		send_client(cli, invalid_no_game);
	}

	std::string msg = "resign|";
	bool is_white = cli->is_white;
	msg.append(is_white ? "white" : "black");
	msg.push_back('\n');

	if (cli->game->clients[0] != nullptr) {
		send_client(cli->game->clients[0], msg.c_str(), msg.size());
	}

	if (cli->game->clients[1] != nullptr) {
		send_client(cli->game->clients[1], msg.c_str(), msg.size());
	}

	// Close down the game and break the link from the clients.
	close_game(cli->game);
}

void server::parse_exit(struct client *cli, char **words, int num_words)
{
	if (cli->game != nullptr) {
		std::string msg = "resign|";
		bool is_white = cli->is_white;
		msg.append(is_white ? "white" : "black");
		msg.push_back('\n');
		send_client(cli->game->clients[!is_white],
				msg.c_str(), msg.size());
	}

	close_client(cli);
}

void server::send_client(struct client *cli, const char msg[])
{
	send_client(cli, msg, strlen(msg));
}

void server::send_client(struct client *cli, const char msg[], int len)
{
#ifdef DEBUG_VERBOSE
	std::cout << msg << std::flush;
#endif
	for (int i = 0; i < len;) {
		int bytes = send(cli->event.data.fd, msg, len, 0);
		if (bytes != -1) {
			i += bytes;
			continue;
		}

		// If this was not due to a client disconnect, then there
		// was a problem that needs to be addressed.
		if (errno != ECONNRESET) {
			std::cerr << "Send: " << strerror(errno) << std::endl;
			return;
		}

		// TODO: Handle Forfeit
		close_client(cli);
	}
}

void server::close_game(struct game *game)
{
	if (game->clients[0] != nullptr) {
		game->clients[0]->game = nullptr;
		game->clients[0]->awaiting_move = false;
		game->clients[0]->is_white = false;
	}

	if (game->clients[1] != nullptr) {
		game->clients[1]->game = nullptr;
		game->clients[1]->awaiting_move = false;
		game->clients[1]->is_white = false;
	}

	delete game->board;

	games.erase(game->game_id);
	delete game;
}

void server::close_client(struct client *cli)
{
	// Close the client file descriptor (this also removes it from epoll).
	close(cli->event.data.fd);
	clients.erase(cli->event.data.fd);
	delete cli;
}

server::~server()
{
	for (const auto & [ key, value ] : clients) {
		delete value;
	}
	clients.clear();

	for (const auto & [ key, value ] : games) {
		delete value;
	}
	games.clear();
}
