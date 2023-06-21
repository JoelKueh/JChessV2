#include "Game.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

Game::Game(int white, int black, std::string *time_str)
{
	// Create the players who will be giving the moves
  	p_white = create_player(white);
  	p_black = create_player(black);
  	parse_time_str(time_str);

	// Create the board to hold the pieces and moves.
  	board = new ChessBoard::BoardRep();
	if (my_options.gui)
		UI = new GameGUI();
	else
		UI = new GameCLI();

	char board_str[8][8];
	board->board_to_strarr(board_str);
	UI->update_pieces(board_str);
}

int sockfd;
void Game::init()
{ 

    int valread;


    //create an address for the socket
    struct sockaddr_in serv_addr;


    // create socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // define address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    serv_addr.sin_addr.s_addr = INADDR_ANY; 

    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))){
        perror("error connecting");
        exit(EXIT_FAILURE);
        }
        



}

int Game::update()
{
	// Handle general updates to the UI.
	refresh();
	int state = UI->update_ui();

	if (state == GameUI::state::U_WRITE) {
		if (UI->write_buf.piece == ' ') {
			board->delete_piece(UI->write_buf.sq);
		} else {
			board->write_piece(UI->write_buf.piece, UI->write_buf.sq);
		}
		char board_str[8][8];
		board->board_to_strarr(board_str);
		UI->update_pieces(board_str);
		int data = htonl(UI->write_buf.piece);
		send(sockfd, &data, sizeof(data), 0);
	}

	// TODO: REFACTOR THIS
	int selected = UI->get_selected_piece();
	if (selected != last_selected_piece) {
		ChessBoard::BoardRep::move_mask moves;
		board->get_mv_mask(&moves, selected);
		UI->set_push_mask(moves.push);
		UI->set_cap_mask(moves.cap);
		UI->set_special_mask(moves.special);
		UI->redraw_pieces();
		last_selected_piece = selected;

	    //char response[256];
	    //recv(sockfd, &response,  sizeof(response), 0);
	    
	    //printf("Server sent: %s", response);


	}

	// Handle updates to the time counts for each player.
	update_player_time();

	// TODO: Handle game state checking.


	// TODO: Handle player move checking.

	// TODO:

	return update_states::M_OK;
}

Player *Game::create_player(int type)
{
	switch (type)
	{
	default:
		return new PlayerLocal();
	case player_type::remote:
		return new PlayerRemote();
	case player_type::AI:
		return new PlayerAI();
	case player_type::TheFish:
		return new PlayerTheFish();
	}
}

void Game::parse_time_str(std::string *time_str)
{
  	int bar_location;
  	for (int i = 0; i < time_str->size(); i++)
  	{
		if ((*time_str)[i] == '|')
		{
			bar_location = i;
			break;
		}
  	}

  	int time = std::stoi(time_str->substr(0, bar_location - 1));
	p_white->time = time;
	p_black->time = time;

  	increment = std::stoi(time_str->substr(bar_location + 2, time_str->size() - bar_location - 2));
}

void Game::update_player_time()
{
	// Handle updates to the time counts for each player.
	auto time_s = std::chrono::system_clock::now();
	auto time_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time_s);
	ulong time_now = time_ms.time_since_epoch().count();
	if(board->white_turn)
	{
		p_white->time = time_now - last_time;
		UI->update_time(p_white->time / 1000, true);
	}
	else
	{
		p_black->time = time_now - last_time;
		UI->update_time(p_white->time / 1000, false);
	}
	last_time = time_now;
}

void handle_player_move()
{
	
}

Scene* Game::create_new()
{
	Scene *new_scene = new StartMenu();
	return new_scene;
}

Game::~Game()
{
	debug_out << "Game Destructed" << std::endl;
}
