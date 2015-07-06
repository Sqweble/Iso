#include "CommonUtilities.h"
#include "stdarg.h"

#pragma warning (disable:4996)

std::string CommonUtilities::GetString_CU(const char* someText,...)
{
	std::string returnString;
	char buffer[1024];
	va_list args;
	va_start(args,someText);
	vsnprintf(buffer,256,someText,args);
	returnString = buffer;
	va_end(args);
	return returnString;
}

void CommonUtilities::AdjustFilePath(std::string& aFilePath)
{
	std::string slashCheck = aFilePath;
	int lastSlash;
	while((lastSlash = slashCheck.find_last_of('\\')) != -1 )
	{
		slashCheck.erase(slashCheck.begin()+lastSlash,slashCheck.end());
		aFilePath.erase(aFilePath.begin()+lastSlash);
		aFilePath.insert(aFilePath.begin()+lastSlash,'/');
	}
}