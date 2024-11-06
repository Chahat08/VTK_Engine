#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "easywsclient.hpp"
#include <assert.h>
#include <stdio.h>
#include <string>

using easywsclient::WebSocket;

class Interactor {
public:
	Interactor(std::string& url);
	~Interactor();
private:
	std::string m_url;
	static WebSocket::pointer m_ws;

	void handle_message(const std::string& message);
	void send(const std::string& message);

};

#endif