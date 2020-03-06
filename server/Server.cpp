#include "Server.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

Server::Server(unsigned short port)
{
	isRunning = true;

	if (socket.bind(port) != sf::Socket::Done)
	{
		std::cerr << "failed to bind to port " << port << std::endl;
		exit(0);
	}

	std::cout << "local ip " << sf::IpAddress::getLocalAddress() << std::endl;
	std::cout << "port" << socket.getLocalPort() << std::endl;

	socket.setBlocking(false);

	room = Room("davis");
	room.loadRoom();

	std::cout << "SERVER READY" << std::endl;
}

Server::~Server()
{
	room.saveRoom();
	std::cout << "closing server" << std::endl;
}

void Server::HandleNetworkEvents()
{
	sf::Packet packet;
	
	sf::IpAddress senderIP;
	unsigned short senderPort;

	if (socket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
	{
		std::string command;
		packet >> command;

		std::cout << senderIP.toString() << ":" << senderPort << ": " << command << std::endl;

		if (command == "requestRoomInformation")
		{
			sf::Packet packet;
			packet << "returnRoomInformation" << room.getRoomInformation();

			if (!socket.send(packet, senderIP, senderPort) == sf::Socket::Done)
				std::cerr << "failed to return room information to client." << std::endl;
		}
		else if (command == "reportUserLogin")
		{
			std::string username = "NO_USER";
			std::time_t loginTime = 0, loginSessionLength = 0;

			#ifdef _WIN32
				packet >> username >> loginTime >> loginSessionLength;
			#else
				packet >> username;

				int lt = lognTime;
				int st = loginSessionLength;

				packet >> lt;
				packet >> st;
			#endif

			std::cout << username << " reported login at " << loginTime << " (session length: " << loginSessionLength << ")" << std::endl;

			if (room.workstations.find(username) == room.workstations.end())
				std::cout << "entry \"" << username << "\" does not exist in workstation list. " << room.workstations.size() << std::endl;
			else
			{
				std::cout << "updating map entry" << std::endl;
				room.workstations[username].status = Workstation::Status::LoggedIn;
			}
		}
		else if (command == "reportUserLogoff")
		{
			std::string username = "NO_USER";
			std::time_t loginTime = 0;
			std::time_t loginSessionLength = 0;

			#ifdef _WIN32
				packet >> username >> loginTime >> loginSessionLength;
			#else
				packet >> username;

				int lt = loginTime;
				int st = loginSessionLength;

				packet >> lt;
				packet >> st;
			#endif

			std::cout << username << " reported logoff at " << loginTime << " (session length: " << loginSessionLength << ")" << std::endl;

			room.workstations[username].status = Workstation::Status::NotLoggedIn;
		}
		else
		{
			std::cerr << "unknown command \"" << command << "\" recieved from " << senderIP << std::endl;
		}
	}
}

void Server::Update()
{
}
