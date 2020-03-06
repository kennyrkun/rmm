#include "AppEngine.hpp"
#include "RoomOverviewState.hpp"

#include "VisualWorkstation.hpp"

#include <SFML/Graphics.hpp>
#include <SFUI/Theme.hpp>

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

RoomOverviewState::RoomOverviewState(const std::string& roomName)
{
	this->roomName = roomName;
}

void RoomOverviewState::Init(AppEngine* app_)
{
	std::cout << "RoomOverviewState initialising" << std::endl;

	app = app_;

	app->window.create(app->settings.graphics.dimensions, "RMM", sf::Style::Default);

	if (!socket.connect(app->settings.server.serverIpAddress, app->settings.server.serverPort))
		std::cout << "connected to server" << std::endl;
	else
		std::cerr << "failed to connect to server" << std::endl;

	rebuildMenu();

	socket.setBlocking(false);

	std::cout << "RoomOverviewState ready" << std::endl;
}

void RoomOverviewState::Cleanup()
{
	std::cout << "Cleaning up RoomOverviewState" << std::endl;

	delete menu;

	std::cout << "Cleaned up RoomOverviewState" << std::endl;
}

void RoomOverviewState::Pause()
{
	std::cout << "RoomOverviewState Pause" << std::endl;
}

void RoomOverviewState::Resume()
{
	std::cout << "RoomOverviewState Resume" << std::endl;
}

void RoomOverviewState::HandleEvents()
{
	sf::Packet packet;
	sf::Socket::Status status = socket.receive(packet);

	switch (status)
	{
	case sf::Socket::Done:
	{
		std::string command;
		packet >> command;

		std::cout << command << std::endl;

		if (command == "notifyRoomUpdate")
		{
			socket.setBlocking(true);
			rebuildMenu();
			socket.setBlocking(false);
		}

		break;
	}
	case sf::Socket::NotReady:
		break;
	case sf::Socket::Partial:
		std::cout << "partial" << std::endl;
		break;
	case sf::Socket::Disconnected:
		std::cerr << "shit" << std::endl;
		app->Quit();
		break;
	case sf::Socket::Error:
		std::cerr << "error" << std::endl;
		break;
	default:
		break;
	}

	sf::Event event;

	while (app->window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			app->Quit();
			return;
		}
	}

	int id = menu->onEvent(event);

	switch (id)
	{
	default:
		break;
	}
}

void RoomOverviewState::Update()
{
}

void RoomOverviewState::Draw()
{
	app->window.clear(SFUI::Theme::windowBgColor);
	app->window.draw(*menu);
	app->window.display();
}

void RoomOverviewState::rebuildMenu()
{
	std::cout << "building menu" << std::endl;

	if (menu)
		delete menu;

	menu = new SFUI::Menu(app->window);
	menu->setPosition(sf::Vector2f(10, 10));
	
	sf::Packet packet;
	packet << "requestRoomInformation";
	if (socket.send(packet) != sf::Socket::Done)
		std::cerr << "failed to send message" << std::endl;
	else
		std::cout << "message sent" << std::endl;

	sf::Packet packetin;
	if (socket.receive(packetin) == sf::Socket::Done)
	{
		std::cout << "received response from server: ";
		std::string command;
		packetin >> command;
		std::cout << command << std::endl;

		std::string information;
		packetin >> information;

		std::cout << information << std::endl;

		std::istringstream iss(information);

		for (std::string line; std::getline(iss, line); )
		{
			std::string token;
			std::stringstream ss(line);

			std::getline(ss, token, ',');
			VisualWorkstation* station = new VisualWorkstation(token);

			std::getline(ss, token, ',');
			station->setStatus(static_cast<VisualWorkstation::Status>(std::stoi(token)));

			menu->add(station);
		}
	}

	std::cout << "menu ready" << std::endl;
}
