#ifndef WINDOWS_HPP
#define WINDOWS_HPP

////////////////////////////////////////////////////////
// PURPOSE: Windows Abstraction.
// This is really just to help keep Windows.h stuff out of my other files.
// It gets really annoying not being able to use certain names because Windows has used them.
// It also seems to make IntelliSense a little bit more snappy.
////////////////////////////////////////////////////////

#include <string>

namespace windows
{
	namespace users
	{
		std::string getCurrentSessionUsername();
	}
}

#endif // !WINDOWS_HPP