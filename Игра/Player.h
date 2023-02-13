#pragma once
#include "Client.h"
#include <vector>

using std::vector;

class Player {
public:
	Player();
	Player(Client);
	Client* getClient();
	/*bool operator==(const Player& another) const {
		return (this->client) == another.client;
	}*/
	bool isLose();
	int give();
	int take(int);
	int getLastTaken();
	int getItemCount();
	void sendLose();
	void sendWin();
	bool lose = false;
private:
	Client client;
	int lastTaken = 0;
	int itemCount = 0;
};

