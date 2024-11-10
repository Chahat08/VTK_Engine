#ifndef CLIENT_H
#define CLIENT_H

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif

#include "easywsclient.hpp"
#include "frontendData.h"

#include <cassert>
#include <iostream>
#include <string>

#include "interaction/interactor.h"

using easywsclient::WebSocket;

class SocketClient {
public:
	SocketClient(std::string& url, int clientID, Interactor* interactor);
	~SocketClient();

	void startPolling();
	void closeConnection();
private:
	std::string m_url;
	int m_client_id;
	static WebSocket::pointer m_ws;

	Interactor* m_interactor;

	void receive(const std::string& message);
	void send(const std::string& message);

};

#endif