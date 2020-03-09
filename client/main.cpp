#include "AppEngine.hpp"
#include "ServerConnectState.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "starting" << std::endl;

	AppSettings settings = { "RMM", "ALPHA", { argc, argv } };
//	AppSettings settings = { { argc, argv } };
	std::cout << "settings ready" << std::endl;

	try
	{
		AppEngine app;
		std::cout << "AppEngine waiting" << std::endl;
		app.Init(settings);

		app.PushState(new ServerConnectState);

		while (app.isRunning())
		{
			app.HandleEvents();
			app.Update();
			app.Draw();
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "failed to create AppEngine." << std::endl;
		std::cerr << e.what() << std::endl;
	}


	return EXIT_SUCCESS;
}
