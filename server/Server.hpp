#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <ctime>
#include <string>

class ClientInformation
{
public:
	sf::IpAddress address;
	std::string username;
	std::time_t loginTime;
	std::time_t loginSessionLength;
};

class Server
{
public:
	Server(unsigned short port = 1234);
	~Server();

	void HandleNetworkEvents();
	void Update();
	void Draw();

	bool isRunning = false;

private:
	sf::UdpSocket socket;

	sf::RenderWindow window;

	std::map<std::string, ClientInformation> clients;

	sf::Font font;

	std::string getTimestamp(std::time_t time = std::time(0));
};

#endif // SERVER_HPP