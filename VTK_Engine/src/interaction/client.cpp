#include "interaction/client.h"
//#include "utils/json.hpp"

WebSocket::pointer SocketClient::m_ws = nullptr;

SocketClient::SocketClient(std::string& m_url, int clientID) {
	INT rc;
	WSADATA wsaData;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		printf("WSAStartup Failed.\n");
	}

	this->m_url = m_url;
	m_ws = WebSocket::from_url(m_url);
	assert(m_ws);
	m_client_id = clientID;
	send("CONNECT " + std::to_string(clientID));
}

void SocketClient::send(const std::string& message) {
	m_ws->send(message);
}

void SocketClient::receive(const std::string& message) {
	std::cout << "Server: " << message << std::endl;

	//if (!message.starts_with("CONNECT")) {
		//nlohmann::json jsonData = nlohmann::json::parse(message);
		/*std::vector<ControlPoint> controlPoints;
		std::vector<ColorStop> colorStops;
		for (const auto& point : jsonData["controlPoints"]) {
			controlPoints.push_back(ControlPoint{ point["x"].get<double>(), point["y"].get<double>() });
		}
		for (const auto& stop : jsonData["colorStops"]) {
			colorStops.push_back({ stop["position"].get<double>(), stop["color"].get<std::string>() });
		}

		std::cout << "Control Points:\n";
		for (const auto& point : controlPoints) {
			std::cout << "x: " << point.x << ", y: " << point.y << "\n";
		}

		std::cout << "\nColor Stops:\n";
		for (const auto& stop : colorStops) {
			std::cout << "position: " << stop.position << ", color: " << stop.color << "\n";
		}*/
	//}
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

void SocketClient::closeConnection() {
	m_ws->close();
	delete m_ws;
}

SocketClient::~SocketClient() {
	m_ws->close();
	delete m_ws;
}