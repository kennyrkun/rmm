#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFUI/SFUI.hpp>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class Client
{
public:
	Client();
	~Client();

	void HandleEvents();
	void Update();
	void Draw();

	int send(sf::Packet packet);

	bool running = false;

private:
	sf::IpAddress address = sf::IpAddress("172.16.115.194");
	unsigned int port = 1234;

	sf::UdpSocket socket;

	sf::RenderWindow window;
};

#endif // !CLIENT_HPP
