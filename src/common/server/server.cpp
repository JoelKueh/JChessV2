#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define PORT 8080

int main (int argc, char *argv[])
{
    char server_message[256] = "You have reached the server\n";

    int server_fd, client_fd, valread;
    struct sockaddr_in address, cli_addr;

    socklen_t clilen;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    //define address structure
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
  
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    clilen = sizeof(cli_addr);

    if ((client_fd
         = accept(server_fd, (struct sockaddr*)&cli_addr,
                  (socklen_t*)&clilen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("server: got connection from %s port %d\n",
            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    int i = 0;
    while(i < 3){

        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }


        if ((client_fd
             = accept(server_fd, (struct sockaddr*)&cli_addr,
                      (socklen_t*)&clilen))
            < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::string a = std::to_string(i);
        send(client_fd, a.c_str(),  sizeof(a), 0);
        i += 1;
    }


    close(server_fd);
  
  return 0;
}
