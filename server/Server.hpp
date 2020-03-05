#ifndef SERVER_HPP
#define SERVER_HPP

#include "Room.hpp"

#include <SFML/Network.hpp>

#include <map>
#include <ctime>
#include <string>

class Server
{
public:
	Server(unsigned short port = 1234);
	~Server();

	void HandleNetworkEvents();
	void Update();

	bool isRunning = false;

private:
	sf::UdpSocket socket;

	Room room;
};

#endif // SERVER_HPP