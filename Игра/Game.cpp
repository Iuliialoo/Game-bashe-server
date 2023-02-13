#include "Game.h"
#include <string>

using namespace std;

Game::Game(vector<Player> players, int N, int M) {
	this->players = players;
	this->N = N;
	this->M = M;
	startGame();
}

void Game::startGame() {
	bool finish = false;
	while (true) {
		for (size_t i = 0; i < players.size(); i++) {
			Client client = ((Client*)players[i].getClient())[0];
			/*string msg = "\nОбщее количество камушков: " + to_string(N);
			client.send_(msg.c_str());
			char buf[1024]{};
			client.recv_(buf, 1);*/
			int choice = randInt(1, 2); // 1 - отдаёт, 2 - берет
			if (choice == 1) {
				int given = players[i].give();
				N += given;
			}
			else if (choice == 2) {
				int takes = players[i].take(M < N ? M : N);
				N -= takes;
				if (N <= 0) {
					players[i].sendLose();
					players[i].lose = true;
					finish = true;
					break;
				}
				else {
					players[i].getClient()->send_("ход переходит к следущему игроку");
				}
			}
		}
		if (finish) break;
	}
	for (size_t i = 0; i < players.size(); i++) {
		if (!players[i].isLose()) {
			players[i].sendWin();
		}
		players[i].getClient()->closeClientSocket();
	}
}

int Game::randInt(int min, int max) {
	return min + rand() % (max - min + 1);
}