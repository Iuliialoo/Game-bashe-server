#pragma once
#include <vector>
#include "Player.h"

class Game {
public:
	Game(vector<Player>, int, int);
private:
	vector<Player> players;
	int N;
	int M;

	void startGame();
	int randInt(int a, int b);
};

