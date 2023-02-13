#include "Player.h"
#include <string>

using namespace std;

Player::Player() {}

Player::Player(Client client) {
	this->client = client;
}

Client* Player::getClient() {
	return &this->client;
}

bool Player::isLose() {
	return this->lose;
}

int Player::give() {
	if (getLastTaken() < 4) {
		client.send_("�� ������ ������ ��������, �� �� �� ������, ��� ��������� � ��������� ������");
		return 0;
	}
	int given = 0;
	char buf[64];
	while (given > lastTaken / 4 || given <= 0) {
		ZeroMemory(buf, 64);
		string msg = "�� ������ ������ ��������, �����-���-�� ���������. �� �� ����� " + to_string(lastTaken / 4);
		client.send_(msg.c_str());
		client.recv_(buf, 64);
		given = atoi(buf);
	}
	client.send_("��� ��������� � ��������� ������");
	return given;
}

int Player::take(int max) {
	int takes = max + 1;
	char buf[64];
	while (takes > max) {
		ZeroMemory(buf, 64);
		string msg = "�� ������ ����� ��������, �����-���-�� ���������, ��������: " + to_string(max);
		client.send_(msg.c_str());
		client.recv_(buf, 64);
		takes = atoi(buf);
	}
	/*client.send_("��� ��������� � ��������� ������");*/
	lastTaken = takes;
	return takes;
}

int Player::getLastTaken() {
	return this->lastTaken;
}

int Player::getItemCount() {
	return this->itemCount;
}

void Player::sendLose() {
	client.send_("�� ��������");
}

void Player::sendWin() {
	client.send_("�� �������");
}