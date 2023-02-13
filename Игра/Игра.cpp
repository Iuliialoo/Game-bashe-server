#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")

#include "headers.h"

using namespace std;

SOCKET thisSocket;
WSAData thisWsaData;
vector<Player> players;
int initPlayersCount = 0;
int initN = 0;
int initM = 0;
bool gameStarted = false;

void startServer(int16_t);
void startListen(sockaddr_in);
void closeServer();
void newClientHandler(Client* client_);
void deleteFromQueue(int);
void checkQueue();
void printQueue();
void startGame();


int main() {
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));

	while (initPlayersCount < 2) {
		cout << "Введите кол-во игроков(>=2): ";
		cin >> initPlayersCount;
	}
	while (initN < initPlayersCount) {
		printf("Введите кол-во преметов на столе(N >= %d): ", initPlayersCount);
		cin >> initN;
	}
	while (initM < 1 || initM > (initN / initPlayersCount)) {
		cout << "Введите максимальное кол-во преметов,\nкоторое игрок может взять за один ход(M > 0 && M < (N / X)): ";
		cin >> initM;
	}
	startServer(PORT);
	return 0;
}

void startServer(int16_t port) {
	if (WSAStartup(MAKEWORD(2, 2), &thisWsaData)) closeServer();
	if ((thisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) closeServer();
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(port);
	local_addr.sin_addr.s_addr = 0;
	if (bind(thisSocket, (sockaddr*)&local_addr, sizeof(local_addr))) closeServer();
	if (listen(thisSocket, 0x100)) closeServer();
	startListen(local_addr);
}

void closeServer() {
	closesocket(thisSocket);
	WSACleanup();
	cout << "Сервер остановлен.." << endl;
}

void startListen(sockaddr_in local_addr) {
	cout << "Слушаем " << PORT << " порт по адресу " << inet_ntoa(local_addr.sin_addr) << endl;
	SOCKET client_socket;
	sockaddr_in client_addr;
	int client_addr_size = sizeof(sockaddr_in);
	while ((client_socket = accept(thisSocket, (sockaddr*)&client_addr, &client_addr_size))) {
		HOSTENT* hst;
		hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);
		DWORD thID;
		Client newClient = Client(client_socket, client_addr);
		thread newClientThread(newClientHandler, &newClient);
		newClientThread.join();
	}
}

void newClientHandler(Client* intputClient_) {
	Client client = ((Client*)intputClient_)[0];
	char clientAnswer[BUF_SIZE] = "\0";

	//if (gameStarted || players.size() >= initPlayersCount) {
	//	cout << client.getUniquePort() << "Пока" << endl;
	//	client.send_("Игра уже началась");
	//	client.closeClientSocket();
	//	return;
	//}

	Player newPlayer = Player(client);
	players.push_back(newPlayer);

	int bytesSent;
	bytesSent = client.send_("Ты добавлен в очередь");
	if (bytesSent < 1) {
		cout << "Ошибка в отправке сообщения " << client.getFullName() << endl;
		deleteFromQueue(client.getUniquePort());
		client.closeClientSocket();
		return;
	}

	string parameters = "Параметры игры:\nКол-во игроков - " + to_string(initPlayersCount);
	parameters += "\nКол-во предметов на столе - " + to_string(initN);
	parameters += "\nМакс кол-во предметов, которое можно взять за один ход - " + to_string(initM);
	parameters += "\nСейчас кол-во игроков в очереди - " + to_string(players.size());
	parameters += '\n';

	bytesSent = client.send_(parameters.c_str());
	/*if (bytesSent < 1) {
		cout << "Ошибка в отправке сообщения " << client.getFullName() << endl;
		deleteFromQueue(client.getUniquePort());
		client.closeClientSocket();
		return;
	}*/
	checkQueue();
}

void deleteFromQueue(int uniquePort) {
	/*int playerIndex = -1;
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i].getClient()->getUniquePort() == uniquePort) playerIndex = i;
	}
	if (playerIndex == -1) return;
	players.erase(players.begin() + playerIndex);*/
}

void checkQueue() {
	/*if (players.size()) {
		for (size_t i = 0; i < players.size(); i++) {
			int bytesSent = players[i].getClient()->send_("check");
			if (bytesSent == SOCKET_ERROR || bytesSent == 0) {
				PRINT_QUEUE;
				printf(" При проверке [%d] произошла ошибка(bytesSend = %d)\n", players[i].getClient()->getUniquePort(), bytesSent);
				printf("[-] [%d] удалён из очереди\n", players[i].getClient()->getUniquePort());
				players[i].getClient()->closeClientSocket();
				deleteFromQueue(players[i].getClient()->getUniquePort());
				PRINT_QUEUE;
			}
		}
		printQueue();
	}*/
	if (players.size() >= initPlayersCount) {
		thread newGame(startGame);
		newGame.join();
	}
}

void printQueue() {
	PRINT_QUEUE;
	printf("[%d] Сейчас очередь - [", players.size());
	for (size_t i = 0; i < players.size(); printf("%d, ", players[i++].getClient()->getUniquePort()));
	printf("]\n");
}

void startGame() {
	gameStarted = true;
	int bytesSent;
	for (size_t i = 0; i < players.size(); i++) {
		bytesSent = players[i].getClient()->send_(START_GAME_PHRASE);
		/*if (bytesSent < 1) {
			cout << "Ошибка в отправке сообщения " << players[i].getClient()->getFullName() << endl;
			deleteFromQueue(players[i].getClient()->getUniquePort());
			players[i].getClient()->closeClientSocket();
			gameStarted = false;
			return;
		}*/
	}
	PRINT_GAME;
	cout << " Началась игра между [";
	for (size_t i = 0; i < players.size(); i++) {
		printf("%d, ", players[i].getClient()->getUniquePort());
	}
	cout << "]" << endl;
	Game newGame = Game(players, initN, initM);
	cout << "Игра закончилась" << endl;
	players.clear();
}