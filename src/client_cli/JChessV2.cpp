
#include "defs.h"

#include <ncurses.h>
#include <iostream>

#ifdef FILE_DEBUG
#include <fstream>
std::ofstream debug_out;
#endif

void spawn_server();
void join_server(std::string ip);
void create_game();
void join_game();
void send_server(std::string msg);

int main(int argc, char *argv[])
{
#ifdef FILE_DEBUG
	debug_out.open("out/output.txt");
#endif

	std::string input;
	std::cout << "Join or spawn server (JOIN/spawn): ";
	std::getline(std::cin, input);
	for (int i = 0; i < input.size(); ++i) {
		input[i] = tolower(input[i]);
	}
	bool spawn_srv = input == "spawn";

	if (spawn_srv) {
		spawn_server();
	} else {
		join_server(input);
	}

	std::cout << "Join or create game (JOIN/create): ";
	std::getline(std::cin, input);
	for (int i = 0; i < input.size(); ++i) {
		input[i] = tolower(input[i]);
	}
	bool create = input == "create";

	if (create) {
		create_game();
	} else {
		join_game();
	}
}

void spawn_server()
{
	// TODO: HANDLE SPAWN
}

void join_server()
{
	std::string input;
	std::cout << "Enter IP Address: ";
	std::getline(std::cin, input);
	
	// TODO: HANDLE JOIN
}

void create_game()
{
	std::string input;
	std::cout << "Enter desired color (WHITE/black)";
	std::getline(std::cin, input);
	for (int i = 0; i < input.size(); ++i) {
		input[i] = tolower(input[i]);
	}
	bool is_whtie = input == "black";

	std::cout << "Enter game time in seconds (0 for untimed): ";
	std::getline(std::cin, input);
	int time = std::stol(input);

	std::cout << "Enter increment in seconds: ";
	std::getline(std::cin, input);
	int incr = std::stol(input);

	// TODO: SEND CREATE GAME MESSAGE
}

void join_game()
{
	// TODO: JOIN GAME
}

void send_server(std::string msg)
{
	// TODO: ERROR HANDLING ON SEND
}
