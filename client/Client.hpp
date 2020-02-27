#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

class Client
{
public:
	Client();
	~Client();

	int send(sf::Packet packet);

private:
	sf::IpAddress address = sf::IpAddress("172.16.115.194");
	unsigned int port = 1234;

	sf::UdpSocket socket;
};

#endif // !CLIENT_HPP
