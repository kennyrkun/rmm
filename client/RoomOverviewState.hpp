#ifndef ROOM_OVERVIEW_STATE_HPP
#define ROOM_OVERVIEW_STATE_HPP

#include "AppState.hpp"

#include <SFUI/SFUI.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class RoomOverviewState : public AppState
{
public:
	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	AppEngine* app;
	// FIXME: shouldn't need to be a pointer

	SFUI::Menu* menu;

	void rebuildMenu();

	sf::UdpSocket socket;
};

#endif // !ROOM_OVERVIEW_STATE_HPP
