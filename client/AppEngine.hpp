#ifndef APP_ENGINE_HPP
#define APP_ENGINE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <vector>
#include <string>

class AppState;

struct AppSettings
{
	std::string title = "voidchat " + version;
	std::string version = "2.5-20w07a";

	struct CommandLineArguments
	{
		int argc;
		char** argv;
	} launchOptions;

	bool debug = true;

	struct Graphics
	{
		sf::VideoMode dimensions = { 1080, 720};
		bool vsync = true;
		bool console = false;
		bool useAnimations = true;
		float animationScale = 1;
		size_t maxfps = 60;
	} graphics;

	struct Locale
	{
		int timezoneOffset = -6;
		bool fullTimeFormat = false;
	} locale;

	struct Server
	{
		sf::IpAddress serverIpAddress = sf::IpAddress::LocalHost;
		unsigned short serverPort = 1234;

		bool connected = false;
	} server;
};

class AppEngine
{
public:
	void Init(AppSettings settings_);
	void Cleanup();

	// Note: code after these functions is still executed.
	void ChangeState(AppState* state);
	void PushState(AppState* state);
	void PopState();
	// amount: the amount of states to pop starting from the back
	void PopState(int amount);

	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning() { return running; }
	void Quit();

	AppSettings settings;

	sf::RenderWindow window;

	std::vector<AppState*> states;
	std::vector<sf::Event> events;

private:
	bool running;
};

#endif // !APP_ENGINE_HPP
