#include "Client.hpp"

#include <SFML/System/Sleep.hpp>

int main(int argc, char* argv[])
{
	Client client;

	while (true)
		sf::sleep(sf::milliseconds(100));

	return 0;
}