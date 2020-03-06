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

void RoomOverviewState::Init(AppEngine* app_)
{
	std::cout << "RoomOverviewState initialising" << std::endl;

	app = app_;

	app->window.create(app->settings.graphics.dimensions, "RMM", sf::Style::Default);

	if (!socket.bind(sf::Socket::AnyPort))
		std::cout << "bound to port: " << socket.getLocalPort() << std::endl;
	else
		std::cerr << "failed to bind to port" << std::endl;

	rebuildMenu();

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
	socket.send(packet, app->settings.server.serverIpAddress, app->settings.server.serverPort);

	sf::IpAddress address;
	unsigned short port;

	sf::Packet packetin;
	if (socket.receive(packetin, address, port) == sf::Socket::Done)
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
