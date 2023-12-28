
#include "client.h"
#include "game_cli.h"

#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#ifdef FILE_DEBUG
#include <fstream>
extern std::ofstream debug_out;
#endif

client::client()
{
	reading_fen = false;
	awaiting_move = false;
	
	// Epoll filedescriptor creation
	epoll_fd = epoll_create(2);
	if (epoll_fd == -1) {
		std::cerr << "Epoll Create: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Add stdin to the epoll_fd
	e_stdin.data.fd = STDIN_FILENO;
	e_stdin.events = EPOLLIN;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e_stdin.data.fd, &e_stdin)) {
		std::cerr << "STDIN Entry Create: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string input;
	std::cout << "Join or spawn server (JOIN/spawn): ";
	std::getline(std::cin, input);
	for (int i = 0; i < input.size(); ++i) {
		input[i] = tolower(input[i]);
	}
	bool spawn_srv = input == "spawn";

	if (spawn_srv) {
		spawn_server();
		std::string ip = "localhost:";
		ip.append(std::to_string(PORT));
		join_server(ip);
	} else {
		std::cout << "Enter IP Address: ";
		std::getline(std::cin, input);
		join_server(input);
	}

	std::cout << "Join or create game (JOIN/create): ";
	std::getline(std::cin, input);
	for (int i = 0; i < input.size(); ++i) {
		input[i] = tolower(input[i]);
	}
	bool create = input == "create";

	if (create) {
		create_game();
	} else {
		join_game();
	}
}

int client::poll_set()
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

int client::handle_events(struct epoll_event *event)
{
	if (event->events & EPOLLRDHUP) {
#if defined(FILE_DEBUG) & defined(DEBUG_VERBOSE)
		debug_out << "SERVER DISCONNECTED" << std::endl;
#endif
		int status = handle_disconnect(event);
		return status;
	}

	if (event->events & EPOLLIN) {
		int status = handle_data(event);
		return status;
	}

#if defined(FILE_DEBUG) & defined(DEBUG_VERBOSE)
	debug_out << "Unexpected Event" << std::endl;
#endif
	return 0;
}

int client::handle_data(struct epoll_event *event)
{
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
		return handle_ncurses(read_buffer, bytes);
	}

	handle_server(read_buffer);
	return 1;
}

int client::handle_ncurses(char *rdbuf, int bytes)
{
	
}

int client::ncurses_char(char input)
{
	int row, col;
	switch (input) {
	case KEY_DOWN:
		row = cli.get_row() + 1;
		row = row > 7 ? 7 : row;
		cli.set_row(row);
		break;
	case KEY_UP:
		row = cli.get_row() - 1;
		row = row < 0 ? 0 : row;
		cli.set_row(row);
		break;
	case KEY_RIGHT:
		col = cli.get_col() + 1;
		col = col > 7 ? 7 : col;
		cli.set_col(col);
		break;
	case KEY_LEFT:
		col = cli.get_col() - 1;
		col = col < 0 ? 0 : col;
		cli.set_col(col);
		break;
	case KEY_ENTER:
	case 'z':
	case 'Z':
		if (move_buf[0] == -1) {
			move_buf.
			break;
		}
		if (move_buf.start_sq == -1) {
			move_buf.start_sq = cursor_row * 8 + cursor_col;
			break;
		}
		move_buf.end_sq = cursor_row * 8 + cursor_col;
		break;
	case 'u':
	case 'U':
		ui_state.unmk_move = true;
		break;
	case ' ':
	case 'P':
	case 'p':
	case 'N':
	case 'n':
	case 'B':
	case 'b':
	case 'R':
	case 'r':
	case 'Q':
	case 'q':
	case 'K':
	case 'k':
		ui_state.write = true;
		write_buf.piece = input;
		write_buf.sq = cursor_row * 8 + cursor_col;
		break;
	defualt:
		break;
	}
}

void client::handle_server(char *rdbuf)
{

}

void client::spawn_server()
{
	int pid = getpid();

	if (fork() == 0) {
		char *args[] = { NULL }; 
		execv("../bin/server", args);
	}
}

void client::join_server(std::string ip)
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Create server socket
	int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "Socket Create: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Translate address string.
	if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
		std::cerr << "Invalid Address: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Connect to server
	if (connect(server_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)) < 0) {
		std::cerr << "Connection Failed: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	e_server.data.fd = server_fd;
	e_server.events = EPOLLIN | EPOLLRDHUP;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &e_server)) {
		std::cerr << "Epoll Socket Add: "
			<< strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void client::create_game()
{
	std::string input;
	bool id_valid = false;
	while (!id_valid) {
		std::cout << "Enter four character game id: ";
		std::getline(std::cin, input);
		id_valid = true;
		for (int i = 0; id_valid && i < input.size(); ++i) {
			id_valid = !(input[i] == '|' || input[i] == '\n'
					|| input[i] == '\0');
		}
	}
	std::string game_id = input;

	std::cout << "Enter desired color (WHITE/black)";
	std::getline(std::cin, input);
	for (int i = 0; i < input.size(); ++i) {
		input[i] = tolower(input[i]);
	}
	bool is_white = input == "black";

	std::cout << "Enter game time in seconds (0 for untimed): ";
	std::getline(std::cin, input);
	int time = std::stol(input);

	std::cout << "Enter increment in seconds: ";
	std::getline(std::cin, input);
	int incr = std::stol(input);

	std::string msg = "create_room|";
	msg.append(game_id);
	msg.push_back('|');
	msg.append(is_white ? "white|" : "black|");
	msg.append(std::to_string(time));
	msg.push_back('|');
	msg.append(std::to_string(incr));
	msg.push_back('\n');
	send_server(msg);
}

void client::join_game()
{
	std::string input;
	bool id_valid = false;
	while (!id_valid) {
		std::cout << "Enter four character game id: ";
		std::getline(std::cin, input);
		id_valid = true;
		for (int i = 0; id_valid && i < input.size(); ++i) {
			id_valid = !(input[i] == '|' || input[i] == '\n'
					|| input[i] == '\0');
		}
	}
	std::string game_id = input;

	std::string msg = "join|";
	msg.append(game_id);
	msg.push_back('\n');
	send_server(msg);
}

void client::send_move(CB::Move move)
{
	std::string algbr = move.to_long_algbr();
	std::string msg = "move|";
	msg.append(algbr);
	msg.push_back('\n');
	send_server(msg);
}

void client::send_resign()
{
	send_server("resign\n");
}

void client::send_exit()
{
	send_server("exit\n");
}

void client::send_server(std::string msg)
{
#if defined(FILE_DEBUG) & defined(DEBUG_VERBOSE)
	debug_out << msg << std::flush;
#endif

	for (int i = 0; i < msg.size();) {
		int bytes = send(e_server.data.fd, msg.c_str(), msg.size(), 0);
		if (bytes != -1) {
			i += bytes;
			continue;
		}

		// If this was not due to a server disconnect, then there
		// was a problem that needs to be addressed.
		if (errno != ECONNRESET) {
			std::cerr << "Send: " << strerror(errno) << std::endl;
			return;
		}

		close_server();
	}
}

client::~client()
{
	close(e_server.data.fd);
}
