#include "Server.hpp"

int main(int argc, char* argv[])
{
	Server server;
	
	while (server.isRunning)
	{
		server.HandleNetworkEvents();
		server.Update();
		server.Draw();

		sf::sleep(sf::milliseconds(100));
	}

	return 0;
}