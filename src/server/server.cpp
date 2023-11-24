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
	char command[] = "abcde|defg|abcesdfpoi|apowie";
	char **words;
	int num_words = split_command(command, &words);
	for (int i = 0; i < num_words; ++i) {
		puts(words[i]);
	}
}
