
#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string>

#include "utils/server.h"

int main(int argc, char *argv[]) {
	server srv;
	while (srv.poll_set());
	std::cout << "Exiting..." << std::endl;
}
