#include "aiden_utils.h"

#include <string>
#include <cstring>

namespace aiden_utils
{
	std::string getErrorMessage(int errorCode)
	{
		using std::string;

		char buf[80];

		strerror_s(buf, errorCode);

		return string{ buf };
	}
}