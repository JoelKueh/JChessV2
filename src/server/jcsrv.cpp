#include <iostream>
#include <string>

#include "../common/CB_2_0/BoardRep.h"
#include "server_utils.h"

#define PORT 24388
#define MAX_CLIENTS 30
#define MAX_GAMES 15

#define MAX_EVENTS 5
#define READ_SIZE 10
#define MAX_COMMAND_LENGTH 50

void parse_command(client &client) {

}

void handle_master(struct epoll_entry &master, struct epoll_entry &epoll,
		struct client clients[MAX_CLIENTS], int nclients)
{
	std::cout << "Client attempted connection" << std::endl;

	struct sockaddr_in *addr = (sockaddr_in *)malloc(sizeof(sockaddr_in));
	int addrlen = sizeof(&addr);
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(PORT);

	int new_socket = accept(master.fd, (struct sockaddr *)&addr,
			(socklen_t*)&addrlen);
	struct epoll_event *event = (epoll_event *)malloc(sizeof(epoll_event));
	event->events = EPOLLIN;
	event->data.fd = new_socket;

	if (nclients < MAX_CLIENTS) {
		clients[nclients++].entry.fd = event->data.fd;
		clients[nclients++].entry.event = event;
	}

	perror("client accept");
	std::cout << "Client File Descriptor: " << new_socket << std::endl;
	if (epoll_ctl(epoll.fd, EPOLL_CTL_ADD, new_socket, event)) {
		perror("client epoll_ctl");
		exit(EXIT_FAILURE);
	}
}

void handle_client(client &client, char read_buffer[READ_SIZE + 1])
{
	for (int i = 0; i < READ_SIZE; ++i) {
		switch (read_buffer[i])
		{
		case '\n':
			client.command[client.comlen++] = '\0';
			parse_command(client);
			break;
		default:
			if (client.comlen >= MAX_COMMAND_LENGTH) {

			client.command[client.comlen
}

int poll_fdset(struct epoll_entry &master, struct epoll_entry &epoll,
		struct client clients[MAX_CLIENTS], struct epoll_event &event,
		int nclients)
{
	int event_count, bytes;
	char read_buffer[READ_SIZE + 1];
	struct epoll_event events[MAX_EVENTS];

	std::cout << "Polling for input...\n";
	event_count = epoll_wait(epoll.fd, events, MAX_EVENTS, -1);
	if (event_count == -1) {
		perror("epoll_wait");
		exit(EXIT_FAILURE);
	}

	std::cout << event_count << " ready events" << std::endl;
	for (int i = 0; i < event_count; ++i) {
		std::cout << "Reading file descriptor "
			<< events[i].data.fd << " -- ";
		bytes = read(events[i].data.fd, read_buffer, READ_SIZE);
		std::cout << bytes << " bytes read." << std::endl;
		read_buffer[bytes] = '\0';
		std::cout << "Read " << read_buffer << std::endl;

		if (events[i].data.fd == 0) {
			if ((std::string)read_buffer == "stop\n")
				return 0;
		} else if (events[i].data.fd == master.fd) {
			handle_master(master, epoll, clients, nclients);
		} else {
			int i = 0;
			while (clients[i].entry.fd != events[i].data.fd
					&& i++ < MAX_CLIENTS);
			handle_client(clients[i - 1], read_buffer);
		}
	}
	std::cout << "Input handled" << std::endl;
	return 1;
}

int main()
{
	// Epoll Data
	int epoll_fd = epoll_create(MAX_CLIENTS + 1);
	if (epoll_fd == -1) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}
	
	// Master Socket Variables 
	int opt = 1;
	int master_soc, addrlen, new_socket;
	int activity, i, valread, sd;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	// Client Socket Variables
	fd_set readfds;
	struct client clients[MAX_CLIENTS] = { 0 };
	CB::BoardRep *games[MAX_GAMES] = { nullptr };
	int nclients = 0;

	// Master Socket Initilization
	if (!(master_soc = socket(AF_INET, SOCK_STREAM, 0))) {
		std::cout << "MASTER SOCKET CREATION FAILED\n"
			<< "TERMINATING" << std::endl;
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}

	if (setsockopt(master_soc, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
				sizeof(opt)) < 0) {
		std::cout << "MASTER SOCKET OPTION SETTING FAILED\n"
			<< "TERMINATING" << std::endl;
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}

	if (bind(master_soc, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		std::cout << "BIND FAILED\nTERMINATING" << std::endl;
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(master_soc, 3) < 0) {
		std::cout << "LISTEN FAILED\nTERMINATING" << std::endl;
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}

	struct epoll_event epl_event;
	epl_event.events = EPOLLIN;
	epl_event.data.fd = 0;
	struct epoll_entry epoll;
	epoll.fd = epoll_fd;
	epoll.event = &epl_event;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, epoll.event)) {
		std::cerr << "Failed to add stdin file descriptor to epoll\n"
			<< "Terminating..." << std::endl;
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}

	struct epoll_event master_event;
	master_event.events = EPOLLIN;
	master_event.data.fd = master_soc;
	struct epoll_entry master;
	master.fd = master_soc;
	master.event = &master_event;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, master_soc, master.event)) {
		std::cerr << "Failed to add master file descriptor to epoll\n"
			<< "Terminating..." << std::endl;
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}

	// Handle input
	while (poll_fdset(master, epoll, clients, epl_event, nclients) > 0);

	// Cleanup
	close(epoll_fd);
	close(master_soc);
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (clients[i].entry.fd != 0)
			close(clients[i].entry.fd);
	}

	exit(EXIT_SUCCESS);
}
