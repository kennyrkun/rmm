#include "Client.hpp"

#include <SFML/Network/Packet.hpp>

#include "Windows.hpp"

#include <iostream>
#include <ctime>

Client::Client()
{
	sf::Packet packet;
	packet << "reportUserLogin";
	packet << windows::users::getUsername();
	packet << std::time(0);
	packet << std::time(0);

	send(packet);
}

Client::~Client()
{
	sf::Packet packet;
	packet << "reportUserLogoff";
	packet << windows::users::getUsername();
	packet << std::time(0);
	packet << std::time(0);

	send(packet);
}

int Client::send(sf::Packet packet)
{
	if (socket.send(packet, address, port) == sf::Socket::Status::Done)
	{
		std::cout << "reported status " << sf::IpAddress("172.16.115.194") << ":" << port << std::endl;
		return 1;
	}
	else
	{
		std::cerr << "failed to send packet" << std::endl;
		return 0;
	}
}