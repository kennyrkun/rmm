#include "AppEngine.hpp"
#include "ServerConnectState.hpp"

int main(int argc, char* argv[])
{
	AppSettings settings = { "RMM", "ALPHA", { argc, argv } };

	AppEngine app;
	app.Init(settings);

	app.PushState(new ServerConnectState);

	while (app.isRunning())
	{
		app.HandleEvents();
		app.Update();
		app.Draw();
	}

	return EXIT_SUCCESS;
}