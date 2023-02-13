#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")
#include "winsock2.h"

class Client {
public:
	Client(SOCKET, sockaddr_in);
	Client(void);
	~Client();
	int send_(const char*);
	int recv_(char*, int len);
	int getUniquePort();
	char* getIP();
	char* getFullName();
	void closeClientSocket();
	bool operator==(const Client& another) const {
		return (this->uniquePort) == another.uniquePort;
	}
private:
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	char* fullName;
	int uniquePort;
	int getCharArrLength(const char*);
};

