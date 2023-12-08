
#ifndef H_CLIENT
#define H_CLIENT

class client
{
private:
	bool reading_fen;

public:
	void send_move();
	void send_join();
	void send_create();
};

#endif
