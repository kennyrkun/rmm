#include <SFML/Network.hpp>

#include "Windows.hpp"

#include <iostream>
#include <ctime>

int main(int argc, char* argv[])
{
	sf::UdpSocket socket;

	sf::Packet packet;
	packet << "reportUserLogin";
	packet << windows::users::getUsername();
	packet << std::time(0);
	packet << std::time(0);

	if (socket.send(packet, sf::IpAddress::getLocalAddress(), 1234) == sf::Socket::Status::Done)
		std::cout << "reported status " << sf::IpAddress::getLocalAddress() << ":1234" << std::endl;
	else
		std::cerr << "failed" << std::endl;

	return 0;
}