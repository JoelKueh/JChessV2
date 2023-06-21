#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

//#include <cstdlib>
//#include <netdb.h>
//#include <netinet/in.h>
#define PORT 8080

// get byte order conversion functions
#include <arpa/inet.h>


int main (int argc, char *argv[])
{

    int sockfd,  valread;


    //create an address for the socket
    struct sockaddr_in serv_addr;


    // create socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // define address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY; 

    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))){
        perror("error connecting");
        exit(EXIT_FAILURE);
        }
        

    char response[256];
    recv(sockfd, &response,  sizeof(response), 0);

    printf("Server sent: %s", response);

    send(sockfd, "1", 1, 0);

    close(sockfd);
    return 0;

} 


       
