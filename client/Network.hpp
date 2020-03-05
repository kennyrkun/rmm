#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <SFML/Network.hpp>

#include <vector>

struct NetworkEvent
{
	const std::string command;

	/*
	std::time_t timeSent;
	std::time_t timeReceived;
	sf::IpAddress sender;
	unsigned short port;
	*/


};

class Network
{
public:
	void pollNetwork(NetworkEvent& event);

private:
	bool sendPacket();
	bool recievePacket(sf::Packet& packet);

	sf::IpAddress address;
	unsigned short port;

	std::vector<NetworkEvent> events;
};

#endif // !NETWORK_HPP
