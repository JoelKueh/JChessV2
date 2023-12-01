#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string>

#include "server_utils.h"

int main(int argc, char *argv[]) {
	struct server srv;
	init_server(&srv);

	while (poll_set(&srv));
	printf("Exiting...\n");
}
