#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <string>
#include <bitset>
#include "FI_File.h"
#include "stdarg.h"
#include "StackWalker.h"
#include "CommonUtilities.h"

template<typename Type, typename SizeType>
class GrowingArray;

using namespace std;

#ifndef RETAIL

#define DL_ASSERT(...)			DL_Debug::GetInstance()->AssertMessage(_CRT_WIDE(__FILE__),__LINE__,_CRT_WIDE(__FUNCTION__),(char*)(CommonUtilities::GetString_CU(__VA_ARGS__).c_str()));
#define DL_PRINT(message)		DL_Debug::GetInstance()->PrintMessage(message);
#define DL_CALLSTACK(message)	DL_Debug::GetInstance()->CallStack(message);
#define DL_DEBUG( ...)			DL_Debug::GetInstance()->DebugMessage(__LINE__,__FUNCTION__,__VA_ARGS__);
#define DL_ERROR( ...)			DL_Debug::GetInstance()->ErrorMessage(__LINE__,__FUNCTION__,__VA_ARGS__);
#define DL_SEVERE( ...)			DL_Debug::GetInstance()->SevereErrorMessage(__LINE__,__FUNCTION__,__VA_ARGS__);
#define DL_CRITICAL( ...)		DL_Debug::GetInstance()->CriticalErrorMessage(__LINE__,__FUNCTION__,__VA_ARGS__);


#define DL_WRITELOG(log, ...) DL_Debug::GetInstance()->WriteLog(log, __VA_ARGS__)
#define DL_TOGGLELOG(log, activationFlag) DL_Debug::GetInstance()->ToggleLog(log, activationFlag)

#define USE_FILTERLOG

#if defined(DEBUG) || defined(_DEBUG)
#define USE_FILTERLOG
#endif

#ifdef USE_FILTERLOG
#define LUA_LOG(...) DL_WRITELOG("Lua", __VA_ARGS__)
#define ENTITY_LOG(...) DL_WRITELOG("Entity", __VA_ARGS__)
#define FMOD_LOG(...) DL_WRITELOG("FMOD", __VA_ARGS__)
#else
#define LUA_LOG(...) (__VA_ARGS__)
#define ENTITY_LOG(...) (__VA_ARGS__)
#define FMOD_LOG(...) (__VA_ARGS__)
#endif
#else
#define DL_ASSERT(...)
#define DL_PRINT(message)
#define DL_CALLSTACK(message)
#define DL_DEBUG( ...)
#define DL_ERROR( ...)
#define DL_SEVERE( ...)
#define DL_CRITICAL( ...)


#define DL_WRITELOG(log, ...)
#define DL_TOGGLELOG(log, activationFlag)

#define USE_FILTERLOG

#if defined(DEBUG) || defined(_DEBUG)
#define USE_FILTERLOG
#endif

#ifdef USE_FILTERLOG
#define LUA_LOG(...)
#define ENTITY_LOG(...)
#define FMOD_LOG(...)
#else
#define LUA_LOG(...)
#define ENTITY_LOG(...)
#define FMOD_LOG(...)
#endif
#endif


enum AsserFlags
{
	ASSERT_DEBUG,
	ASSERT_ERROR,
	ASSERT_SEVERE,
	ASSERT_CRITICAL
};

struct DebugLogStruct
{
	std::string myLogName;
	bool myIsActivated;

	DebugLogStruct()
	:	myLogName("")
	,	myIsActivated(false)
	{
	}

	DebugLogStruct(const std::string& aLogName, bool anIsActivatedFlag)
	:	myLogName(aLogName)
	,	myIsActivated(anIsActivatedFlag)
	{
	}
};

class DL_Debug
{
public:
	static bool Create(std::string aFile = "DebugLog.txt", std::bitset<4> someAssertFlags = 12);
	static bool Destroy();
	static DL_Debug* GetInstance();
	void AssertMessage(wchar_t* aFilePath,int aLine,wchar_t* aFunction, char* aMessage);
	void AssertMessage(wchar_t* aFilePath,int aLine,wchar_t* aFunction, wchar_t* aMessage);
	void DebugMessage(int aLine,char* aFunction, ...);
	void PrintMessage(string aString);
	void CallStack(string aString);
	void ErrorMessage(int aLine,char* aFunction, ...);
	void SevereErrorMessage(int aLine,char* aFunction, ...);
	void CriticalErrorMessage(int aLine,char* aFunction, ...);
	void WriteLog(const char* aLogName,...);
	void ToggleLog(const char* aLogName, bool anActivationFlag);

private:
	DL_Debug(void);
	~DL_Debug(void);	
	static bool Init(std::string aFile,std::bitset<4> someAssertFlags);
	static void WriteMessage(int aLine,char* aFunction ,va_list someArgs);
	int FindLog(const char* aLogName);
private:
	
	static DL_Debug* ourInstance;
	static File myDebugFile;
	static bitset<4> myAssertFlags;	
	
	GrowingArray<DebugLogStruct,int>* myLogs;

};

#endif