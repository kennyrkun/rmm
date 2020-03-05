#ifndef INITIALISE_STATE_HPP
#define INITIALISE_STATE_HPP

#include "AppState.hpp"

#include <SFML/Graphics.hpp>

class ServerConnectState : public AppState
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

	bool isReady = false;

	void initialise();

	sf::Font font;
	sf::Text initialiseText;
	sf::RectangleShape thingthatdoesntripoffvisualstudio;

	sf::Clock initTime;
};

#endif // !INITIALISE_STATE_HPP
