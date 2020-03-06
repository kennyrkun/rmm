#ifndef WORKSTATION_HPP
#define WORKSTATION_HPP

#include <SFML/Network/IpAddress.hpp>

#include <ctime>
#include <string>

class Workstation
{
public:
	enum Status
	{
		LoggedIn,
		NotLoggedIn,
		Unlocked,
		Locked,
	} status = Status::NotLoggedIn;

	/*
	std::string machineName;
	std::string networkName;
	*/

	std::string username;
	std::time_t loginTime;
	std::time_t loginSessionLength;
	sf::IpAddress address;
};

#endif // !WORKSTATION_HPP
