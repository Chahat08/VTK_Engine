#ifndef INTERACTOR_H
#define INTERACTOR_H

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif

#include "easywsclient.hpp"
#include "frontendData.h"

#include <cassert>
#include <iostream>
#include <string>


using easywsclient::WebSocket;

class SocketClient {
public:
	SocketClient(std::string& url, int clientID);
	~SocketClient();

	void startPolling();
	void closeConnection();
private:
	std::string m_url;
	int m_client_id;
	static WebSocket::pointer m_ws;

	void receive(const std::string& message);
	void send(const std::string& message);

};

#endif