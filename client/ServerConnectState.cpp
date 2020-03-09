#include "AppEngine.hpp"
#include "ServerConnectState.hpp"
#include "RoomOverviewState.hpp"

#include <SFML/Graphics.hpp>
#include <SFUI/Theme.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void ServerConnectState::Init(AppEngine* app_)
{
	initTime.restart();
	std::cout << "ServerConnectState Init" << std::endl;

	app = app_;

	// HACK: this is nasty
//	font = GBL::theme.getFont("Arial.ttf");
//	if (font == nullptr)
	{
		std::cerr << "failed to load provided arial, falling back to windows Arial!" << std::endl;

		if (!font.loadFromFile("C:/Windows/Fonts/Arial.ttf"))
			std::cerr << "failed to load font Arial!" << std::endl;
	}

	app->window.create(sf::VideoMode(400, 150), app->settings.title, sf::Style::None);
	app->window.setVerticalSyncEnabled(app->settings.graphics.vsync);
	app->window.setTitle("rmm initalising");

	initialiseText.setFont(font);
	initialiseText.setCharacterSize(56);
	initialiseText.setString("rmm");
	initialiseText.setOrigin(sf::Vector2f(initialiseText.getLocalBounds().width / 2.0f, initialiseText.getLocalBounds().height / 2));
	initialiseText.setPosition(sf::Vector2f(static_cast<int>(app->window.getView().getCenter().x), static_cast<int>(app->window.getView().getCenter().y / 2.0f)));

	thingthatdoesntripoffvisualstudio.setSize(sf::Vector2f(app->window.getSize().x, 20.0f));
	thingthatdoesntripoffvisualstudio.setFillColor(sf::Color(100, 100, 100));
	thingthatdoesntripoffvisualstudio.setPosition(sf::Vector2f(0.0f, (app->window.getSize().y - 20.0f)) );

	// draw once and then forget about it
	Draw();

	initialise();

	app->ChangeState(new RoomOverviewState("davis"));
}

void ServerConnectState::Cleanup()
{
	std::cout << "Cleaning up ServerConnectState" << std::endl;
	std::cout << "Initalisation took " << initTime.getElapsedTime().asSeconds() << "s" << std::endl;
	std::cout << "Cleaned up ServerConnectState" << std::endl;
}

void ServerConnectState::Pause()
{
	std::cout << "ServerConnectState Pause" << std::endl;
}

void ServerConnectState::Resume()
{
	std::cout << "ServerConnectState Resume" << std::endl;
}

void ServerConnectState::HandleEvents()
{
	sf::Event event;
	while (app->window.pollEvent(event))
		std::cout << "ignoring event while initialising: " << event.type << std::endl;
}

void ServerConnectState::Update()
{
}

void ServerConnectState::Draw()
{
	app->window.clear(sf::Color(50, 50, 50));
	app->window.draw(initialiseText);
	app->window.draw(thingthatdoesntripoffvisualstudio);
	app->window.display();
}

void ServerConnectState::initialise()
{
	SFUI::Theme::loadFont("./rmm/resource/fonts/arial.ttf");
	SFUI::Theme::loadTexture("./rmm/resource/textures/square.png");

	SFUI::Theme::textCharacterSize = 11;
	SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#CC7A00");
	SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#dddbde");
	SFUI::Theme::PADDING = 2.f;
}
