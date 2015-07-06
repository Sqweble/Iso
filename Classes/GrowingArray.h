#ifndef _GROWING_ARRAY_
#define _GROWING_ARRAY_

#include "windows.h"
#include "DL_ASSERT.h"

template<typename ObjectType, typename SizeType = int>
class GrowingArray
{
public:
	GrowingArray(void);
	GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);
	GrowingArray(const GrowingArray& aGrowingArray);
	virtual ~GrowingArray(void);

	GrowingArray& operator=(const GrowingArray& aGrowingArray);

	void Init(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true);
	void ReInit(SizeType aNrOfRecomendedItems,bool aUseSafeModeFlag=true);

	inline void Add(const ObjectType& aObject);
	inline void Add();
	inline void Insert(int aIndex,const ObjectType& aObject);

	inline void DeleteCyclic(const ObjectType& aObject);
	inline void DeleteCyclicAtIndex(int aIndex);
	inline void DeleteAll();
	inline void DeleteLast();

	inline void RemoveCyclic(const ObjectType& aObject);
	inline void RemoveCyclicAtIndex(int aIndex);
	inline void RemoveOrdered(int aIndex);
	inline void RemoveRange(int aIndex, int aRange);
	inline void RemoveAll();
	inline void RemoveLast();

	inline SizeType Find(const ObjectType& aObject) const;
	inline ObjectType& operator[](const SizeType& aIndex);
	inline const ObjectType& operator[](const SizeType& aIndex) const;
	inline ObjectType& At(const SizeType& aIndex);
	inline const ObjectType& At(const SizeType& aIndex) const;

	void Optimize();

	__forceinline int Count() const;

	inline ObjectType& GetLast();
	inline const ObjectType& GetLast() const;

	ObjectType* GetArray() const;

	inline void SetUsedSize(const int aSize);

	static const SizeType FoundNone = -1;

private:
	inline void ReSize(int aNewSize);

	SizeType mySize;
	SizeType myUsedSize;
	bool mySafeModeFlag;
	bool myIsOptimized;
	ObjectType* myArray;
};

template<typename ObjectType, typename SizeType>
GrowingArray<ObjectType,SizeType>::GrowingArray(void)
{
	mySize = 0;
	myUsedSize = 0;
	mySafeModeFlag = false;
	myIsOptimized = false;
	myArray = nullptr;
}

template<typename ObjectType, typename SizeType>
GrowingArray<ObjectType,SizeType>::GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag)
{
	mySize = aNrOfRecommendedItems;
	myUsedSize = 0;
	mySafeModeFlag = aUseSafeModeFlag;
	myIsOptimized = false;
	myArray = new ObjectType[mySize];
}

template<typename ObjectType, typename SizeType>
GrowingArray<ObjectType,SizeType>::GrowingArray(const GrowingArray& aGrowingArray)
{
	mySize = 0;
	myUsedSize = 0;
	mySafeModeFlag = false;
	myIsOptimized = false;
	myArray = nullptr;
	*this = aGrowingArray;
}

template<typename ObjectType, typename SizeType>
GrowingArray<ObjectType,SizeType>::~GrowingArray(void)
{
	mySize = 0;
	myUsedSize = 0;
	mySafeModeFlag = false;
	myIsOptimized = false;
	delete[] myArray;
	myArray = nullptr;
}

template<typename ObjectType, typename SizeType>
GrowingArray<ObjectType,SizeType>& GrowingArray<ObjectType,SizeType>::operator=(const GrowingArray& aGrowingArray)
{
	mySize = aGrowingArray.mySize;
	mySafeModeFlag = aGrowingArray.mySafeModeFlag;
	myUsedSize = aGrowingArray.myUsedSize;
	if(myArray != nullptr)
	{
		delete[] myArray;
		myArray = nullptr;
	}
	myArray = new ObjectType[mySize];
	if(mySafeModeFlag == true)
	{
		for(SizeType i=0;i<myUsedSize;i++)
		{
			myArray[i] = aGrowingArray.myArray[i];
		}
	}
	else
	{
		memcpy(myArray,aGrowingArray.myArray,sizeof(ObjectType) * myUsedSize);
	}
	return *this;
}

template<typename ObjectType, typename SizeType>
void GrowingArray<ObjectType,SizeType>::Init(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag)
{
	if(myArray == nullptr)
	{
		mySafeModeFlag = aUseSafeModeFlag;
		aNrOfRecommendedItems = (aNrOfRecommendedItems == 0)? aNrOfRecommendedItems+1 : aNrOfRecommendedItems;
		myArray = new ObjectType[aNrOfRecommendedItems];
		mySize = aNrOfRecommendedItems;
	}
	else
	{
		ReInit(aNrOfRecommendedItems,aUseSafeModeFlag);
	}
}

template<typename ObjectType, typename SizeType>
void GrowingArray<ObjectType,SizeType>::ReInit(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag)
{
	assert(myArray != nullptr && "Init intended");
	delete[] myArray;
	myArray = nullptr;
	myUsedSize = 0;
	mySafeModeFlag = aUseSafeModeFlag;
	myArray = new ObjectType[aNrOfRecommendedItems];
	mySize = aNrOfRecommendedItems;
}



template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::Add(const ObjectType& aObject)
{
 	if(myUsedSize >= mySize)
	{
		ReSize(mySize*2);
	}
	myArray[myUsedSize] = aObject;
	myUsedSize++;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::Add()
{
	if(myUsedSize >= mySize)
	{
		ReSize(mySize*2);
	}
	myArray[myUsedSize] = ObjectType();
	myUsedSize++;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::Insert(int aIndex,const ObjectType& aObject)
{
	if(myUsedSize >= mySize)
	{
		ReSize(mySize*2);
	}
	for(SizeType i = myUsedSize;i>aIndex;i--)
	{
		myArray[i] = myArray[i-1];
	}
	myArray[aIndex] = aObject;
	myUsedSize++;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::DeleteCyclic(const ObjectType& aObject)
{
	for(SizeType i=0;i<myUsedSize;i++)
	{
		if(aObject == myArray[i])
		{
			if(myArray[i] != nullptr)
			{
				delete(myArray[i]);
			}
			myArray[i] = nullptr;
			myArray[i] = myArray[myUsedSize-1];
			myUsedSize--;
			break;
		}
	}	
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::DeleteCyclicAtIndex(int aIndex)
{
	assert(aIndex >= 0 && "Index Out of Bounds");
	assert(aIndex < myUsedSize && "Index Out of Bounds");

	delete myArray[aIndex];
	myArray[aIndex] = myArray[myUsedSize-1];
	myUsedSize--;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::DeleteAll()
{
	for(SizeType i=0;i<myUsedSize;i++)
	{
		if(myArray[i] != nullptr)
		{
			delete myArray[i];
		}
		myArray[i] = nullptr;
	}
	myUsedSize = 0;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::DeleteLast()
{
	assert(myUsedSize > 0);

	delete myArray[myUsedSize-1];
	myUsedSize--;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::RemoveCyclic(const ObjectType& aObject)
{
	for(SizeType i=0;i<myUsedSize;i++)
	{
		if(aObject == myArray[i])
		{
			myArray[i] = myArray[myUsedSize-1];
			myUsedSize--;
			break;
		}
	}
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::RemoveCyclicAtIndex(int aIndex)
{
	assert(aIndex >= 0 && "Index Out of Bounds");
	assert(aIndex < myUsedSize && "Index Out of Bounds");

	myArray[aIndex] = myArray[myUsedSize-1];
	myUsedSize--;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::RemoveOrdered(int aIndex)
{
	assert(aIndex >= 0 && "Index Out of bounds");
	assert(aIndex < myUsedSize && "Index Out of bounds");

	for(int i=aIndex+1;i<myUsedSize;i++)
	{
		myArray[i-1] = myArray[i];
	}
	myUsedSize--;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::RemoveRange(int aIndex, int aRange)
{
	GrowingArray<ObjectType,SizeType>& curArray = *this;
	GrowingArray<ObjectType,SizeType> tempArray(myUsedSize);
	for(int i=aIndex+aRange;i<myUsedSize;i++)
	{
		tempArray.Add(curArray[i]);
	}

	curArray = tempArray;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::RemoveAll()
{
	myUsedSize = 0;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::RemoveLast()
{
	assert(myUsedSize >= 0 && "Cannot remove element from empty array.");
	myUsedSize--;
}

template<typename ObjectType, typename SizeType>
inline SizeType GrowingArray<ObjectType,SizeType>::Find(const ObjectType& aObject) const
{
	SizeType found = FoundNone;
	for(SizeType i=0;i<myUsedSize;i++)
	{
		if(aObject == myArray[i])
		{
			found = i;
		}
	}
	return found;
}

template<typename ObjectType, typename SizeType>
inline ObjectType& GrowingArray<ObjectType,SizeType>::operator[](const SizeType& aIndex)
{
	assert(myArray != nullptr && "Array not initialized");
	assert(aIndex >= 0 && "Index Out of Bounds");
	assert(aIndex < myUsedSize && "Index Out of Bounds");

	return myArray[aIndex];
}

template<typename ObjectType, typename SizeType>
inline const ObjectType& GrowingArray<ObjectType,SizeType>::operator[](const SizeType& aIndex) const
{
	assert(myArray != nullptr && "Array not initialized");
	assert(aIndex >= 0 && "Index Out of Bounds");
	assert(aIndex < myUsedSize && "Index Out of Bounds");

	return myArray[aIndex];
}

template<typename ObjectType, typename SizeType>
inline ObjectType& GrowingArray<ObjectType,SizeType>::At(const SizeType& aIndex)
{
	assert(myArray != nullptr && "Array not initialized");
	assert(aIndex >= 0 && "Index Out of Bounds");
	assert(aIndex < myUsedSize && "Index Out of Bounds");

	return myArray[aIndex];
}

template<typename ObjectType, typename SizeType>
inline const ObjectType& GrowingArray<ObjectType,SizeType>::At(const SizeType& aIndex) const
{
	assert(myArray != nullptr && "Array not initialized");
	assert(aIndex >= 0 && "Index Out of Bounds");
	assert(aIndex < myUsedSize && "Index Out of Bounds");

	return myArray[aIndex];
}

template<typename ObjectType, typename SizeType>
void GrowingArray<ObjectType,SizeType>::Optimize()
{
	ReSize(myUsedSize);
	myIsOptimized = true;
}

template<typename ObjectType, typename SizeType>
__forceinline int GrowingArray<ObjectType,SizeType>::Count() const
{
	return static_cast<int>(myUsedSize);
}

template<typename ObjectType, typename SizeType>
inline ObjectType& GrowingArray<ObjectType,SizeType>::GetLast()
{
	assert(myUsedSize > 0 && "Out of bounds");
	return myArray[myUsedSize-1];
}

template<typename ObjectType, typename SizeType>
inline const ObjectType& GrowingArray<ObjectType,SizeType>::GetLast() const
{
	assert(myUsedSize > 0 && "Out of bounds");
	return myArray[myUsedSize-1];
}

template<typename ObjectType, typename SizeType>
ObjectType* GrowingArray<ObjectType,SizeType>::GetArray() const
{
	assert(myArray != nullptr && "Array is not initialized");
	return myArray;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::SetUsedSize(const int aSize)
{
	myUsedSize = aSize;
}

template<typename ObjectType, typename SizeType>
inline void GrowingArray<ObjectType,SizeType>::ReSize(int aNewSize)
{
	assert(myArray != nullptr && "Array is not initialized");
	/*if(myIsOptimized == true)
	{
		aNewSize = mySize+1;
	}*/

	if( aNewSize < mySize )
	{
		mySize = aNewSize;
	}

	ObjectType* tempArray = new ObjectType[aNewSize];
	if(mySafeModeFlag == true)
	{
		for(SizeType i=0;i<mySize;i++)
		{
			tempArray[i] = myArray[i];
		}
	}
	else
	{
		memcpy(tempArray,myArray,sizeof(ObjectType) * mySize);
	}
	mySize = aNewSize;
	delete[] myArray;
	myArray = tempArray;
	tempArray = nullptr;
}

#endif