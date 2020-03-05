#ifndef ROOM_HPP
#define ROOM_HPP
#include "Workstation.hpp"

#include <string>
#include <map>

class Room
{
public:
	Room();
	Room(const std::string& name);

	std::string name = "default";

	bool loadRoom();
	bool saveRoom();

	const std::string getRoomInformation();

	std::map<std::string, Workstation> workstations;
};

#endif // !ROOM_HPP
