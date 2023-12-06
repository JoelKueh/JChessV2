#ifndef H_SERVER_UTILS
#define H_SERVER_UTILS

#include <string.h>
#include "server_defs.h"
#include "game_utils.h"

void init_server(struct server *srv);
void cleanup(struct server *srv);

int poll_set(struct server *srv);
int handle_event(struct server *srv, struct epoll_event *evt);
void handle_master(struct server *srv);
void handle_client(struct server *srv, struct client *client, char rb[]);

int split_command(char command[], char ***words);
void parse_command(struct server *srv, struct client *client);
enum command_type parse_head(char *word);
void parse_move(struct server *srv, struct client *client,
		char **words, int num_words);
void parse_join(struct server *srv, struct client *client,
		char **words, int num_words);
void parse_create_room(struct server *srv, struct client *client,
		char **words, int num_words);

void send_client(struct client *client, const char msg[]);

#endif /* H_SERVER_UTILS */
