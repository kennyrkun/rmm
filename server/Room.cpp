#include "Room.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

Room::Room()
{
}

Room::Room(const std::string& name)
{
	this->name = name;
	// create room with the name
}

bool Room::loadRoom()
{
	std::ifstream getLayout("./room_" + name + ".csv", std::ios::in | std::ios::binary);

	std::cout << "loading session" << std::endl;

	if (getLayout.is_open())
	{
		std::string line = "line";
		std::getline(getLayout, line);
		while (std::getline(getLayout, line))
		{
			std::cout << line << std::endl;

			std::string token;
			std::stringstream ss(line);

			Workstation workstation;

			std::getline(ss, token, ',');
			workstation.username = token;

			std::getline(ss, token, ',');
			workstation.loginTime = std::stoi(token);

			std::getline(ss, token, ',');
			workstation.loginSessionLength = std::stoi(token);

			std::getline(ss, token, ',');
			workstation.address = sf::IpAddress(token);

			workstations.emplace(workstation.username, workstation);
		}

		std::cout << "loaded session layout" << std::endl;
		return true;
	}
	else
	{
		std::cerr << "failed to open session layout for reading" << std::endl;
		return false;
	}
}

bool Room::saveRoom()
{
	std::ofstream saveCurrent("./room_" + name + ".csv", std::ios::out | std::ios::trunc);

	if (saveCurrent.is_open())
	{
		saveCurrent << "Username,Login Time,Session Length, IP Address" << std::endl;

		for (const auto&[username, info] : workstations)
			saveCurrent << username << "," << info.loginTime << "," << info.loginSessionLength << "," << info.address << std::endl;

		saveCurrent.close();

		if (saveCurrent.bad())
		{
			std::cerr << "something went wrong while saving, the file may be incomplete" << std::endl;
			return false;
		}

		return true;
	}
	else
	{
		std::cerr << "failed to open list for writing" << std::endl;
		return false;
	}
}

const std::string Room::getRoomInformation()
{
	std::ifstream fileContent("./room_" + name + ".csv", std::ios::in | std::ios::binary);

	if (fileContent.is_open())
		return std::string((std::istreambuf_iterator<char>(fileContent)), std::istreambuf_iterator<char>());
	else
	{
		std::cerr << "failed to open list for writing" << std::endl;
		return "broke";
	}
}
