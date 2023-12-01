// Debug
#include <iostream>

#include "server_utils.h"
#include "game_utils.h"

void init_server(struct server *srv)
{
	// Epoll filedecriptor creation
	srv->epoll_fd = epoll_create(MAX_CLIENTS + 2);
	if (srv->epoll_fd == -1) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}

	// Master Socket Initilization
	if (!(srv->master.fd = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	int opt = 1;
	int addrlen = sizeof(addr);
	if (setsockopt(srv->master.fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
				sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	if (bind(srv->master.fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(srv->master.fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Adding stdin to the epoll_fd
	srv->sin.event->events = EPOLLIN;
	srv->sin.event->data.fd = srv->sin.fd;
	if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, srv->sin.fd,
				srv->sin.event)) {
		perror("epoll_ctl");
		exit(EXIT_FAILURE);
	}

	// Adding the master socket to the epoll_fd
	srv->master.event->events = EPOLLIN;
	srv->master.event->data.fd = srv->master.fd;
	if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, srv->master.fd,
				srv->master.event)) {
		perror("epoll_ctl");
		exit(EXIT_FAILURE);
	}
}

int poll_set(struct server *srv)
{
	int event_count, bytes;
	char read_buffer[READ_SIZE + 1];
	struct epoll_event events[MAX_EVENTS];

	// Polling for input...
	event_count = epoll_wait(srv->epoll_fd, events, MAX_EVENTS, -1);
	if (event_count == -1) {
		perror("epoll_wait");
		exit(EXIT_FAILURE);
	}

	// Cycling through ready events
	for (int i = 0; i < event_count; ++i) {
		bytes = read(events[i].data.fd, read_buffer, READ_SIZE);
		read_buffer[bytes] = '\0';
		
		if (events[i].data.fd == 0) {
			if (strcmp(read_buffer, "stop\n"))
				return 0;
		} else if (events[i].data.fd == srv->master.fd) {
			handle_master(srv);
		} else {
			int i = 0;
			while (srv->clients[i].entry.fd != events[i].data.fd
					&& i++ < MAX_CLIENTS);
			handle_client(&srv->clients[i - 1], read_buffer);
		}
	}
	return 1;
}

void handle_master(struct server *srv)
{

}

void handle_client(struct server *srv, struct client *client, char rb[])
{
	const char invalid_msg[] = "invalid_command";

	int i = 0;
	char ch;
	while ((ch = rb[i++])) {
		if (ch == '\n') {
			client->command[client->comlen++] = '\0';
			parse_command(srv, client);
			client->comlen = 0;
		} else if (ch == '\0') {
			send(client, invalid_msg);
		} else {
			if (client->comlen++ == MAX_COMMAND_LENGTH) {
				send(client, invalid_msg);
				client->comlen = 0;
			}
			client->command[client->comlen++] = ch;
		}
	}
}

void parse_command(struct server *srv, struct client *client)
{
	const char invalid_msg[] = "invalid_command";

	char **words;
	int num_words, i;
	num_words = split_command(client->command, &words);
	enum command_type type = parse_head(words[0]);
	
	switch (type)
	{
	case command_type::MOVE:
		parse_move(srv, client, words,  num_words);
		break;
	case command_type::JOIN:
		parse_join(srv, client, words, num_words);
		break;
	case command_type::CREATE_ROOM:
		parse_create_room(srv, client, words, num_words);
		break;
	case command_type::UPDATE:
		break;
	default:
		send(client, invalid_msg);
	}
}

enum command_type parse_head(char *head)
{
	const char move[] = "move";
	const char join[] = "join";
	const char create_room[] = "create_room";
	const char update[] = "update";

	if (strcmp(head, move) == 0) {
		return command_type::MOVE;
	} else if (strcmp(head, join) == 0) {
		return command_type::JOIN;
	} else if (strcmp(head, create_room) == 0) {
		return command_type::CREATE_ROOM;
	} else if (strcmp(head, update) == 0) {
		return command_type::UPDATE;
	} else {
		return command_type::INVALID;
	}
}

void parse_move(struct server *srv, struct client *client,
		char **words, int num_words)
{
	const char invalid_msg[] = "invalid_command";
	const char invalid_move[] = "invalid_move";

	if (num_words != 2) {
		send(client, invalid_msg);
		return;
	}

	std::string algbr = words[1];
	CB::Move move = algbr_to_move(*client->game->board, algbr);

	if (move.is_invalid()) {
		send(client, invalid_move);
		return;
	}

	client->game->board->make(move);
}

void parse_join(struct server *srv, struct client *client,
		char **words, int num_words)
{
	const char invalid_game_id[] = "invalid_game_id";
	const char game_full[] = "invalid_game_already_full";
	const char invalid_msg[] = "invalid_msg";
	int i;

	// Prevent out of bounds access on words.
	if (num_words != 2) {
		send(client, invalid_msg);
	}
	
	char *game_id = words[1];
	for (int i = 0; i < srv->num_games; ++i) {
		if (strcmp(game_id, srv->games[i]->game_id) == 0) {
			game *this_game = srv->games[i];
			if (this_game->clients[0] == nullptr) {
				this_game->clients[0] = client;
				client->is_white = false;
			} else if (this_game->clients[1] == nullptr) {
				this_game->clients[1] = client;
				client->is_white = true;
			} else {
				send(client, game_full);
				return;
			}
			client->game = this_game;
			client->awaiting_move = false;
		}
	}
}

void parse_create_room(struct server *srv, struct client *client,
		char **words, int num_words)
{
	const char invalid_game_id[] = "invalid_game_id";
	const char invalid_msg[] = "invalid_msg";
	const char white[] = "white";

	// Prevent out of bounds access on words.
	if (num_words != 5) {
		send(client, invalid_msg);
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
		send(client, invalid_game_id);
		return;
	}

	// Inputs are validated, proceed with creating the new game.
	struct game *new_game = (game*)malloc(sizeof(game));
	memcpy(new_game->game_id, game_id, 5);
	client->is_white = (strcmp(color, white) == 0);
	new_game->clients[client->is_white] = client;
	new_game->times[0] = strtol(time, nullptr, 10) * 1000;
	new_game->times[1] = new_game->times[0];
	new_game->is_timed = (new_game->times[0] != 0);
	new_game->incr = strtol(incr, nullptr, 10) * 1000;
	new_game->board = new CB::BoardRep;

	srv->games[srv->num_games++] = new_game;
	client->game = new_game;
	client->awaiting_move = false;
}

int split_command(char command[], char ***words)
{
	// Count words in command and allocate an array of start positions.
	int num_words = 1;
	char *c = command;
	while (*c) { num_words += (*c++ == '|' ? 1 : 0); }
	*words = (char**)malloc(num_words * sizeof(char*));

	// Step through command character by character, when you come across
	// a '|', replace the '|' with a '\0' and set the next 
	int word_index = 0, i = 0;
	char ch;
	(*words)[word_index] = command;
	while (word_index < num_words - 1) {
		if ((*words)[word_index][i] == '|') {
			(*words)[word_index][i] = '\0';
			(*words)[word_index + 1] = (*words)[word_index] + i + 1;
			word_index++;
			i = 0;
		}
		++i;
	}
	return num_words;
}

void send(struct client *client, const char msg[])
{
	// Debug: Write directly to console
	std::cout << msg << std::endl;
}

void cleanup(struct server *srv)
{

}
