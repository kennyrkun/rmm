#ifndef WORKSTATION_HPP
#define WORKSTATION_HPP

#include <SFML/Network/IpAddress.hpp>

#include <ctime>
#include <string>

class Workstation
{
public:
	enum class Status
	{
		Active,
		Inactive,
		Locked,
	} status = Status::Inactive;

	std::string username;
	std::time_t loginTime;
	std::time_t loginSessionLength;
	sf::IpAddress address;
};

#endif // !WORKSTATION_HPP
