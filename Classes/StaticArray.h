#ifndef _STATIC_ARRAY_
#define _STATIC_ARRAY_

#include "DL_Assert.h"

template<typename ObjectType,int Size>
class StaticArray
{
public:
	StaticArray(void);
	StaticArray(const StaticArray& aStaticArray);
	~StaticArray(void);

	StaticArray& operator=(const StaticArray& aStaticArray);

	inline ObjectType& operator[](const int& aIndex);

	inline const ObjectType& operator[](const int& aIndex) const;

	inline void Insert(int aIndex,ObjectType& aObject);

	ObjectType* GetArray();

	inline void DeleteAll();

private:
	ObjectType myArray[Size];
};

template<typename ObjectType,int Size>
StaticArray<ObjectType,Size>::StaticArray()
{
}

template<typename ObjectType,int Size>
StaticArray<ObjectType,Size>::StaticArray(const StaticArray& aStaticArray)
{
	*this = aStaticArray;
}

template<typename ObjectType,int Size>
StaticArray<ObjectType,Size>::~StaticArray()
{
}

template<typename ObjectType,int Size>
StaticArray<ObjectType,Size>& StaticArray<ObjectType,Size>::operator=(const StaticArray& aStaticArray)
{
	for(int i=0;i<Size;i++)
	{
		myArray[i] = aStaticArray[i];
	}
	return *this;
}

template<typename ObjectType,int Size>
inline ObjectType& StaticArray<ObjectType,Size>::operator[](const int& aIndex)
{
	assert(aIndex >= 0 && "Out of bounds");
	assert(aIndex < Size && "Out of Bounds");
	return myArray[aIndex];
}

template<typename ObjectType,int Size>
inline const ObjectType& StaticArray<ObjectType,Size>::operator[](const int& aIndex) const
{
	assert(aIndex >= 0 && "Out of bounds");
	assert(aIndex < Size && "Out of Bounds");
	return myArray[aIndex];
}

template<typename ObjectType,int Size>
inline void StaticArray<ObjectType,Size>::Insert(int aIndex,ObjectType& aObject)
{
	assert(aIndex >= 0 && "Out of bounds");
	assert(aIndex < Size && "Out of Bounds");
	for(int i=Size-1;i>aIndex;i--)
	{
		myArray[i] = myArray[i-1];
	}
	myArray[aIndex] = aObject;
}

template<typename ObjectType,int Size>
ObjectType* StaticArray<ObjectType,Size>::GetArray()
{
	return &myArray[0];
}

template<typename ObjectType,int Size>
inline void StaticArray<ObjectType,Size>::DeleteAll()
{
	for(int i=0;i<Size;i++)
	{
		delete myArray[i];
		myArray[i] = NULL;
	}
}

#endif