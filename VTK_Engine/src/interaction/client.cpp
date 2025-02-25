#include "interaction/client.h"


WebSocket::pointer SocketClient::m_ws = nullptr;

SocketClient::SocketClient(std::string& m_url, std::string clientID, Interactor* interactor, std::pair<double, double> valueRange) {
#ifdef _WIN32
	INT rc;
	WSADATA wsaData;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		printf("WSAStartup Failed.\n");
	}
#endif

	this->m_url = m_url;
	m_ws = WebSocket::from_url(m_url);
	if (!m_ws) return; // render anyway
	assert(m_ws);
	m_client_id = clientID;
	send(clientID);
	// sending the intensity range of the volume to the server
	send("valRange " + std::to_string(valueRange.first) + " " + std::to_string(valueRange.second));
	std::cout << "SENT THE STUFF" << std::endl;
	m_interactor = interactor;
	m_interactor->setTerminateCallback([this]() { closeConnection(); });
}

void SocketClient::send(const std::string& message) {
	m_ws->send(message);
}

void SocketClient::receive(const std::string& message) {
	m_interactor->handleServerMessage(message);
}

void SocketClient::startPolling() {
	while (m_ws->getReadyState() != WebSocket::CLOSED) {
		m_ws->poll();
		//m_ws->dispatch(receive);
		m_ws->dispatch([this](const std::string& message) {
			this->receive(message);
			});
	}
	delete m_ws;
}

std::string SocketClient::getID() {
	return m_client_id;
}

void SocketClient::closeConnection() {
	m_ws->close();
}

SocketClient::~SocketClient() {
	delete m_ws;
}