#ifndef SERVER_HPP
#define SERVER_HPP

#include "Room.hpp"

#include <SFML/Network.hpp>

#include <map>
#include <ctime>
#include <string>
#include <vector>

class Server
{
public:
	Server(unsigned short udpPort = 1234, unsigned short tcpPort = 12345);
	~Server();

	void HandleNetworkEvents();
	void Update();

	void tcpBroadcastMessage(const std::string& command, const std::string& message);

	bool isRunning = false;

private:
	// used for talking to management clients
	sf::TcpListener tcpListener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> tcpSockets;

	// used for talking to regular clients
	sf::UdpSocket udpSocket;

	Room room;
};

#endif // SERVER_HPP