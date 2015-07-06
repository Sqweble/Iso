#ifndef _FILE_WRAPPER_
#define _FILE_WRAPPER_

#include <string>
#include <bitset>

//#include "Vector3.h"
//#include "Matrix.h"

enum FileFlags
{
	READ,
	WRITE,
	APPEND,
	BINARY,

	F_READ = 1,
	F_WRITE = 2,
	F_APPEND = 4,
	F_BINARY = 8
};

class File
{
public:
	File(void);
	~File(void);

	bool Open(std::string aFileName, std::bitset<8> aSetOfFlags=0xF);
	bool Close();

	bool GoToNewLine();

	int GetFileSize();

	bool ReadInt(int &aInteger);
	bool ReadFloat(float &aFloat);
	bool ReadDouble(double &aDoubble);
	bool ReadWord(std::string &aString);
	bool ReadString(std::string &aString);

//	bool ReadVector3f(Vector3f& aVector);
//	bool ReadMatrix44f(Matrix44f& aMatrix);

	bool WriteString(const std::string &aString);
	bool WriteInt(int aInteger);
	bool WriteFloat(float aFloat);
	bool WriteDouble(double aDouble);
	
//	bool WriteVector3f(const Vector3f& aVector);
//	bool WriteMatrix44f(const Matrix44f& aMatrix);

	bool WriteData(void *someData,int aSizeToWrite);
	bool ReadData(void *someData,int aSizeToRead);

	template<typename Type>
	bool Write(const Type& aType);
	template<typename Type>
	bool Read(Type& aType);
	bool FlushToDisk();

private:

	FILE *myFile;
	std::bitset<8> myFlags;
	int mySize;
};

template<typename Type>
bool File::Write(const Type& aType)
{
	if(myFlags.test(BINARY))
	{
		fwrite(&aType,sizeof(Type),1,myFile);
	}
	else
	{
		assert(false && "This only works for binary!");
	}
	return true;
}

template<typename Type>
bool File::Read(Type& aType)
{
	if(myFlags.test(BINARY))
	{
		fread(&aType,sizeof(Type),1,myFile);
	}
	else
	{
		assert(false && "This only works for binary!");
	}
	return true;
}

#endif