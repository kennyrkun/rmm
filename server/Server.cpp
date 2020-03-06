#include "Server.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

Server::Server(unsigned short udpPort, unsigned short tcpPort)
{
	isRunning = true;

	if (udpSocket.bind(udpPort) != sf::Socket::Status::Done)
	{
		std::cerr << "failed to bind to port " << udpPort << std::endl;
		exit(0);
	}

	if (tcpListener.listen(tcpPort) != sf::Socket::Status::Done)
	{
		std::cerr << "failed to bind to port " << tcpPort << std::endl;
		exit(0);
	}

	std::cout << "local ip:" << sf::IpAddress::getLocalAddress() << std::endl;
	std::cout << "public ip:" << sf::IpAddress::getPublicAddress() << std::endl;
	std::cout << "UDP Port: " << udpSocket.getLocalPort() << std::endl;
	std::cout << "TCP Port: " << tcpListener.getLocalPort() << std::endl;

	room = Room("davis");
	room.loadRoom();

	selector.add(tcpListener);
	selector.add(udpSocket);

	std::cout << "SERVER READY" << std::endl;
}

Server::~Server()
{
	room.saveRoom();
	std::cout << "closing server" << std::endl;
}

void Server::HandleNetworkEvents()
{
	// selector.wait returns true if something inside of it is ready
	// returns false if nothing is ready
	if (selector.wait())
	{
		std::cout << "ready: ";

		if (selector.isReady(tcpListener))
		{
			std::cout << "tcpListener" << std::endl;

			sf::TcpSocket* socket = new sf::TcpSocket;

			if (tcpListener.accept(*socket) == sf::Socket::Status::Done)
			{
				tcpSockets.push_back(socket);
				selector.add(*socket);
				std::cout << "accepted TCP socket from tcpListener" << std::endl;
			}
			else
			{
				std::cout << "failed to accept TCP socket from tcpListener" << std::endl;
				// TODO: this stuff might cause a crash and break it
				// but it's also unlikely that this code will ever be called,
				// so i don't see much of a reason to try to fix it right now
				// 2020/03/6
				socket->disconnect();
				selector.remove(*socket);
				delete socket;
				tcpSockets.erase(std::remove(tcpSockets.begin(), tcpSockets.end(), socket));
				std::cout << "removed TCP socket from tcpListener" << std::endl;
			}
		}
		else if (selector.isReady(udpSocket))
		{
			std::cout << "udpPacket" << std::endl;

			sf::Packet packet;

			sf::IpAddress senderIP;
			unsigned short senderPort;

			if (udpSocket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
			{
				std::string command;
				packet >> command;

				std::cout << senderIP.toString() << ":" << senderPort << ": " << command << std::endl;

				if (command == "requestRoomInformation")
				{
					sf::Packet packet;
					packet << "returnRoomInformation" << room.getRoomInformation();

					if (!udpSocket.send(packet, senderIP, senderPort) == sf::Socket::Done)
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
						
						int lt = loginTime;
						int st = loginSessionLength;

						packet >> lt;
						packet >> st;
					#endif

					std::cout << username << " reported login at " << loginTime << " (session length: " << loginSessionLength << ")" << std::endl;

					if (room.workstations.find(username) == room.workstations.end())
						std::cout << "entry \"" << username << "\" does not exist in workstation list. " << room.workstations.size() << std::endl;
					else
						room.workstations[username].status = Workstation::Status::LoggedIn;

					tcpBroadcastMessage("notifyRoomUpdate", "uww");
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

					if (room.workstations.find(username) == room.workstations.end())
						std::cout << "entry \"" << username << "\" does not exist in workstation list. " << room.workstations.size() << std::endl;
					else
						room.workstations[username].status = Workstation::Status::NotLoggedIn;

					tcpBroadcastMessage("notifyRoomUpdate", "uww");
				}
				else
				{
					std::cerr << "unknown command \"" << command << "\" recieved from " << senderIP << std::endl;
				}
			}
		}
		else
		{
			for (auto& socket : tcpSockets)
			{
				if (selector.isReady(*socket))
				{
					std::cout << socket->getRemoteAddress() << ":" << socket->getRemotePort() << " (tcpSocket)" << std::endl;

					sf::Packet packet;

					if (socket->receive(packet) == sf::Socket::Done)
					{
						std::string command;
						packet >> command;

						std::cout << socket->getRemoteAddress() << ":" << socket->getRemotePort() << ": " << command << std::endl;

						if (command == "requestRoomInformation")
						{
							sf::Packet packet;
							packet << "returnRoomInformation" << room.getRoomInformation();

							if (socket->send(packet) != sf::Socket::Done)
								std::cerr << "failed to return room information to client." << std::endl;
						}
					}
					else if (socket->receive(packet) == sf::Socket::Disconnected)
					{
						selector.remove(*socket);
						socket->disconnect();

						delete socket;
						tcpSockets.erase(std::remove(tcpSockets.begin(), tcpSockets.end(), socket));

						std::cout << "removed TCP socket from tcpListener, becasue it reported disconnection" << std::endl;
					}
				}
			}
		}
	}
}

void Server::Update()
{
}

void Server::tcpBroadcastMessage(const std::string& command, const std::string& message)
{
	for (auto& socket : tcpSockets)
	{
		sf::Packet packet;

		packet << command << message;

		if (socket->send(packet) != sf::Socket::Done)
			std::cerr << "failed to return room information to client." << std::endl;
	}
}
