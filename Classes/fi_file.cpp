#include "FI_File.h"
#include "Assert.h"

File::File(void)
:	myFile(NULL)
{
}

File::~File(void)
{
}

#pragma warning(disable : 4996)

bool File::Open(std::string aFileName, std::bitset<8> aSetOfFlags)
{
	if(myFile != NULL)
	{
		return false;
	}
	std::string flags;
	if(aSetOfFlags.test(READ))
	{
		if(aSetOfFlags.test(WRITE))
		{
			flags += "r+";
		}
		else
		{
			flags += "r";
		}
	}
	else
	{
		if(aSetOfFlags.test(WRITE))
		{
			if(aSetOfFlags.test(APPEND))
			{
				flags += "a+";
			}
			else
			{
				flags += "w";
			}
		}
		else
		{
			assert(aSetOfFlags.test(APPEND));
			flags += "a";
		}
	}
	if(aSetOfFlags.test(BINARY))
	{
		flags += "b";
	}
	myFile = fopen(aFileName.c_str(), flags.c_str());
	myFlags = aSetOfFlags;

	if(myFile == NULL)
	{
		return false;
	}

	fseek(myFile , 0 , SEEK_END);
	mySize = ftell (myFile);
	rewind (myFile);

	return true;
}

bool File::Close()
{
	if(myFile != NULL)
	{
		fclose(myFile);
		myFile = nullptr;
	}
	else
	{
		return false;
	}
	return true;
}

bool File::GoToNewLine()
{
	char input;
	do
	{
		fread(&input,1,1,myFile);
	}while(input != '\n');
	return true;
}

int File::GetFileSize()
{
	return mySize;
}

bool File::ReadInt(int &aInteger)
{
	if(myFlags.test(BINARY))
	{
		fread(static_cast<void*>(&aInteger),sizeof(int),1,myFile);
	}
	else
	{
		std::string inputBuffer;
		unsigned char input;
		bool found = true;
		do {
			if(fread(&input,1,1,myFile) != 1)
			{
				aInteger = atoi(inputBuffer.c_str());
				return false;
			}
			found = false;
			if(isdigit(input) || input == '-')
			{
				inputBuffer += input;
				found=true;
			}
		} while ((inputBuffer.size() == 0) || (found==true));
		aInteger = atoi(inputBuffer.c_str());
	}
	return true;
}

bool File::ReadFloat(float &aFloat)
{
	if(myFlags.test(BINARY))
	{
		fread(static_cast<void*>(&aFloat),sizeof(float),1,myFile);
	}
	else
	{
		std::string inputBuffer;
		unsigned char input;
		bool found = true;
		do {
			if(fread(&input,1,1,myFile) != 1)
			{
				aFloat = static_cast<float>(atof(inputBuffer.c_str()));
				return false;
			}
			found = false;
			if(isdigit(input) || (input == '-') || (input == '.'))
			{
				inputBuffer += input;
				found = true;
			}
		}while((inputBuffer.size() == 0) || (found==true));
		aFloat = static_cast<float>(atof(inputBuffer.c_str()));
	}
	return true;
}

bool File::ReadDouble(double &aDouble)
{
	if(myFlags.test(BINARY))
	{
		fread(static_cast<void*>(&aDouble),sizeof(double),1,myFile);
	}
	else
	{
		std::string inputBuffer;
		unsigned char input;
		bool found = true;
		do {
			if(fread(&input,1,1,myFile) != 1)
			{
				aDouble = static_cast<double>(atof(inputBuffer.c_str()));
				return false;
			}
			found = false;
			if(isdigit(input) || (input == '-') || (input == '.'))
			{
				inputBuffer += input;
				found = true;
			}
		}while((inputBuffer.size() == 0) || (found==true));
		aDouble = static_cast<double>(atof(inputBuffer.c_str()));
	}
	return true;
}

bool File::ReadWord(std::string &aString)
{
	if(myFlags.test(BINARY))
	{
		fread(static_cast<void*>(&aString),sizeof(std::string),1,myFile);
	}
	else
	{
		std::string inputBuffer;
		unsigned char input;
		char lastNonNumber = '0';
		bool found = true;
		do {
			if(fread(&input,1,1,myFile) != 1)
			{
				aString = inputBuffer;
				return false;
			}
			found = false;
			if((((isdigit(input) == false) || (isdigit(lastNonNumber) == false)) || (input == '-')) && input != ' ' && input != '\n')
			{
				if(isdigit(input) == false)
				{
					lastNonNumber = input;
				}
				inputBuffer += input;
				found = true;
			}
		} while ((inputBuffer.size() == 0) || (found==true));
		aString = inputBuffer;
	}
	return false;
}

bool File::ReadString(std::string &aString)
{
	if(myFlags.test(BINARY))
	{
		fread(static_cast<void*>(&aString),sizeof(std::string),1,myFile);
	}
	else
	{
		std::string inputBuffer;
		unsigned char input;
		bool found = true;
		do {
			if(fread(&input,1,1,myFile) != 1)
			{
				aString = inputBuffer;
				return false;
			}
			found = false;
			if(input == '"' && input != '\n')
			{
				do {
					fread(&input,1,1,myFile);
					if(input != '"')
					{
						inputBuffer += input;
					}
					else
					{
						break;
					}
					}while(ftell(myFile) > 0);
				break;
			}
		}while(ftell(myFile) > 0);
		aString = inputBuffer;
	}
	return true;
}

//bool File::ReadVector3f(Vector3f& aVector)
//{
//	if(myFlags.test(BINARY))
//	{
//		fread(static_cast<void*>(&aVector.x),sizeof(float),3,myFile);
//	}
//	else
//	{
//		assert(false && "This only works for binary!");
//	}
//	return true;
//}
//
//bool File::ReadMatrix44f(Matrix44f& aMatrix)
//{
//	if(myFlags.test(BINARY))
//	{
//		fread(static_cast<void*>(&aMatrix.myMatrix[0]),sizeof(float),16,myFile);
//	}
//	else
//	{
//		assert(false && "This only works for binary!");
//	}
//	return true;
//}

bool File::WriteString(const std::string &aString)
{
	if( fwrite(aString.c_str(),sizeof(char),aString.size(),myFile) != aString.size())
	{
		return false;
	}
	return true;
}

bool File::WriteInt(int aInteger)
{
	if(myFlags.test(BINARY))
	{
		fwrite(&aInteger,sizeof(int),1,myFile);
	}
	else
	{
		char buffer[256];
		sprintf(buffer,"%i", aInteger);
		std::string stringBuffer(buffer);
		fwrite(stringBuffer.c_str(),sizeof(char),stringBuffer.size(),myFile);
		char tempchar = ' ';
		fwrite(&tempchar,sizeof(char),1,myFile);
	}
	return true;
}

bool File::WriteFloat(float aFloat)
{
	if(myFlags.test(BINARY))
	{
		fwrite(&aFloat,sizeof(float),1,myFile);
	}
	else
	{
		char buffer[256];
		sprintf(buffer,"%f", aFloat);
		std::string stringBuffer(buffer);
		fwrite(stringBuffer.c_str(),sizeof(char),stringBuffer.size(),myFile);
		char tempchar = ' ';
		fwrite(&tempchar,sizeof(char),1,myFile);
	}
	return true;
}

bool File::WriteDouble(double aDouble)
{
	if(myFlags.test(BINARY))
	{
		fwrite(&aDouble,sizeof(double),1,myFile);
	}
	else
	{
		char buffer[256];
		sprintf(buffer,"%g", aDouble);
		std::string stringBuffer(buffer);
		fwrite(stringBuffer.c_str(),sizeof(char),stringBuffer.size(),myFile);
		char tempchar = ' ';
		fwrite(&tempchar,sizeof(char),1,myFile);
	}
	return true;
}

//bool File::WriteVector3f(const Vector3f& aVector)
//{
//	if(myFlags.test(BINARY))
//	{
//		fwrite(&aVector.x,sizeof(float),3,myFile);
//	}
//	else
//	{
//		assert(false && "Only works with binary!");
//	}
//	return true;
//}
//
//bool File::WriteMatrix44f(const Matrix44f& aMatrix)
//{
//	if(myFlags.test(BINARY))
//	{
//		fwrite(&aMatrix.myMatrix[0],sizeof(float),16,myFile);
//	}
//	else
//	{
//		assert(false && "Only works with binary!");
//	}
//	return true;
//}

bool File::WriteData(void *someData,int aSizeToWrite)
{
	fwrite(someData,sizeof(char),aSizeToWrite,myFile);
	return true;
}

bool File::ReadData(void *someData,int aSizeToRead)
{
	fread(someData,sizeof(char),aSizeToRead,myFile);
	return false;
}

bool File::FlushToDisk()
{
	fflush(myFile);
	return true;;
}