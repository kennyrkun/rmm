#ifndef ROOM_OVERVIEW_STATE_HPP
#define ROOM_OVERVIEW_STATE_HPP

#include "AppState.hpp"

#include <SFUI/SFUI.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class RoomOverviewState : public AppState
{
public:
	RoomOverviewState(const std::string& roomName);

	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	SFUI::Menu* menu = nullptr;

	std::string roomName;

	sf::TcpSocket socket;

	void rebuildMenu();
};

#endif // !ROOM_OVERVIEW_STATE_HPP
