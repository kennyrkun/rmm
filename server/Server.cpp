#include "Server.hpp"

#include <SFML/Graphics.hpp>

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

	window.create(sf::VideoMode(800, 600), "rmm-server");

	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

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
	sf::Event event;
	window.pollEvent(event);

	if (event.type == sf::Event::Closed)
	{
		std::cout << "window closed, exiting" << std::endl;
		isRunning = false;
		return;
	}

	sf::Packet packet;
	
	sf::IpAddress senderIP;
	unsigned short senderPort;

	if (socket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
	{
		std::string command;
		packet >> command;

		if (command == "reportUserLogin")
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
			{
				std::cout << "updating map entry" << std::endl;
				room.workstations[username].status = Workstation::Status::Active;
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

			room.workstations[username].status = Workstation::Status::Inactive;
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

void Server::Draw()
{
	window.clear();

	const float padding = 10.0f;
	const float height = 40.0f;
	const float width = 40.0f;

	int loops = 0;
	for (const auto& [username, info] : room.workstations)
	{
		sf::RectangleShape shape;
		shape.setPosition(sf::Vector2f(padding, (height * loops) + padding * loops));
		shape.setSize(sf::Vector2f(width, height));

		if (info.status == Workstation::Status::Active)
			shape.setFillColor(sf::Color::Green);
		else if (info.status == Workstation::Status::Locked)
			shape.setFillColor(sf::Color::Yellow);
		else
			shape.setFillColor(sf::Color::Red);

		window.draw(shape);

		sf::Text text;
		text.setFont(font);
		text.setString(room.workstations[username].username + " : " + std::to_string(room.workstations[username].loginTime));
		text.setPosition(shape.getPosition() + sf::Vector2f(width + padding, 0));
		window.draw(text);
		loops++;
	}

	sf::Text text;
	text.setFont(font);
	text.setString(std::to_string(room.workstations.size()));
	window.draw(text);

	window.display();
}

std::string Server::getTimestamp(std::time_t time)
{
	time_t $time = time;

	int seconds = $time % 60;
	$time /= 60;

	int minutes = $time % 60;
	$time /= 60;

	int hours = $time % 24;
	$time /= 24;

	// 12 hour time format
	if (hours > 12)
		hours -= 12;

	signed int timezoneOffset = -6;

	hours += timezoneOffset;

	std::string seconds_s(std::to_string(seconds));
	std::string minutes_s(std::to_string(minutes));
	std::string hours_s(std::to_string(hours));

	if (seconds < 10)
		seconds_s.insert(0, "0");

	if (minutes < 10)
		minutes_s.insert(0, "0");

	/*
	if (hours < 10)
		hours_s.insert(0, "0");
	*/

	std::string timestamp = hours_s + ":" + minutes_s + ":" + seconds_s;

	return timestamp;
}
