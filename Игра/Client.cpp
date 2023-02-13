#include "Client.h"
#include <string>
#include <iostream>

using namespace std;

Client::Client() {
}

Client::Client(SOCKET socket, sockaddr_in addr) {
	this->clientSocket = socket;
	this->clientAddr = addr;
	this->uniquePort = clientAddr.sin_port;
	string clientFullName = string(getIP()) + "[" + to_string(getUniquePort()) + "]";
	fullName = new char[clientFullName.length() + 1];
	strcpy_s(fullName, clientFullName.length() + 1, clientFullName.c_str());
	cout << fullName << " только что подключился" << endl;
	send_(string("Hello " + string(fullName)).c_str());
	char buf[32];
	recv_(buf, 32);
	send_(string("Your id " + to_string(getUniquePort())).c_str());
}

Client::~Client() {
}

int Client::send_(const char* buf) {
	return send(clientSocket, buf, getCharArrLength(buf), 0);
}

int Client::recv_(char* buf, int len) {
	return recv(clientSocket, &buf[0], len, 0);
}

int Client::getUniquePort() {
	return this->uniquePort;
}

char* Client::getIP() {
	return inet_ntoa(clientAddr.sin_addr);
}

char* Client::getFullName() {
	return fullName;
}

int Client::getCharArrLength(const char* buf) {
	int len = 0;
	for (len; buf[len] != '\0'; len++);
	return len;
}

void Client::closeClientSocket() {
	printf("Закрытие связи с [%d]\n", getUniquePort());
	closesocket(clientSocket);
}