#ifndef _COMMON_UTILITIES_HEADER_
#define _COMMON_UTILITIES_HEADER_

#include <string>

namespace CommonUtilities
{
	std::string GetString_CU(const char* someText,...);
	void AdjustFilePath(std::string& aFilePath);
};

#endif