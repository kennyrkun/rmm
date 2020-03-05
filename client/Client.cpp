#include "Client.hpp"

#include "Windows.hpp"

#include <iostream>
#include <ctime>

Client::Client()
{
	running = true;
}

Client::~Client()
{
	running = false;
}

void Client::HandleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			window.close();
		 	// possibly inform server that management client has disconnected
		}
	}
}

void Client::Update()
{
}

void Client::Draw()
{
	window.clear();
	window.display();
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