#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <sys/time.h>

#include "../common/CB_2_0/BoardRep.h"

#define PORT 24388
#define MAX_CLIENTS 30
#define MAX_GAMES 15

struct client {
	int fd;
	CB::BoardRep *board;
};

int main()
{
	int opt = 1;
	int master_soc;
	struct client clients[MAX_CLIENTS] = { 0 };
	CB::BoardRep *games[MAX_GAMES] = { 0 };

	struct sockaddr_storage addr;
	addr.

	if (!(master_soc = socket(AF_INET, SOCK_STREAM, 0))) {
		std::cout << "MASTER SOCKET CREATION FAILED\n"
			<< "TERMINATING" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (setsockopt(master_soc, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
				sizeof(opt)) < 0) {
		std::cout << "MASTER SOCKET OPTION SETTING FAILED\n"
			<< "TERMINATING" << std::endl;
		exit(EXIT_FAILURE);
	}

	
	if (bind(master_soc, 
}
