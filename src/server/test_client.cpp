// SHAMELESSLY STOLEN FROM GEEKSFORGEEKS
// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 24388 

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr,
					sizeof(serv_addr))) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	char command[101];
	command[100] = '\0';
	while (true) {
		fgets(command, 100, stdin);
		printf("Command: %s\n", command);
		send(client_fd, command, strlen(command), 0);
	}

	// closing the connected socket
	close(client_fd);
	return 0;
}
