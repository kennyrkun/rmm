#include "Windows.hpp"

#include <windows.h>
#include <Lmcons.h>

std::string windows::users::getUsername()
{
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);

	return std::string(username);
}
