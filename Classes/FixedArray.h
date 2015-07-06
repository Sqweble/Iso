#ifndef COMMONUTILITIES_FIXEDARRAY_HEADER
#define COMMONUTILITIES_FIXEDARRAY_HEADER

#include "assert.h"
#include <string>

#ifndef NULL
#define NULL 0
#endif

template <class Type,class CountType =unsigned short>
class FixedArray
{
public:
	FixedArray();
	FixedArray(int aNrOfRecomendedItems);
	FixedArray(const FixedArray& aGrowingArray);
	~FixedArray();

	FixedArray& operator=(const FixedArray& aGrowingArray);


	void Init(int aNrOfRecomendedItems);
	void ReInit(int aNrOfRecomendedItems);

	inline Type& operator[](const int& aIndex) const;

	inline void Add(const Type& aObject);
	inline void Insert(int aIndex,Type& aObject);
	inline void DeleteCyclic(Type& aObject);
	inline void DeleteCyclicAtIndex(int aItemNumber);
	inline void RemoveCyclic(const Type& aObject);
	inline void RemoveCyclicAtIndex(int aItemNumber);


	inline void RemoveAll();
	inline void DeleteAll();

	__forceinline int Count() const;

	void* GetArray() const;

	void SetCurrentNrOfItems(int aNumber);

protected:

	Type* myItemList;

	CountType myCurNrOfItems;
	CountType myMaxNrOfItems;

};

template <class Type,class CountType>
FixedArray<Type,CountType>::FixedArray()
{
	myItemList=NULL;
	myCurNrOfItems=0;
	myMaxNrOfItems=0;
}

template <class Type,class CountType>
FixedArray<Type,CountType>::FixedArray(int aNrOfRecomendedItems)
{
	myItemList=NULL;
	myCurNrOfItems=0;
	Init(aNrOfRecomendedItems);
}

template <class Type,class CountType>
FixedArray<Type,CountType>::FixedArray(const FixedArray<Type,CountType>& aGrowingArray)
{
	*this=aGrowingArray;
}

template <class Type,class CountType>

FixedArray<Type,CountType>& FixedArray<Type,CountType>::operator=(const FixedArray<Type,CountType>& aFixedArray)
{
	ReInit(aFixedArray.myMaxNrOfItems);


	for(int i=0;i<aFixedArray.Count();i++)
	{
		myItemList[i]=aFixedArray[i];
	}
	myCurNrOfItems=aFixedArray.Count();
	return(*this);
}



template <class Type,class CountType>
FixedArray<Type,CountType>::~FixedArray()
{
	RemoveAll();
	if(myItemList!=NULL) delete [] (myItemList);
	myItemList=NULL;
}


template <class Type,class CountType>
void FixedArray<Type,CountType>::Init(int aNrOfRecomendedItems)
{
	if(myItemList == nullptr)
	{
		assert(myItemList==NULL && "CALLED INIT ON AN ALREADY INITED FIXEDARRAY PLEASE USE REINIT");

		myMaxNrOfItems=aNrOfRecomendedItems;
		myCurNrOfItems=0;

		myItemList = new Type [myMaxNrOfItems] ();
	}
	else
	{
		ReInit(aNrOfRecomendedItems);
	}
}

template <class Type,class CountType>
void FixedArray<Type,CountType>::ReInit(int aNrOfRecomendedItems)
{
	assert(myItemList!=NULL && "CALLED REINIT ON AN UNINITED GROWINGARRAY PLEASE USE INIT");

	delete [] (myItemList);

	myMaxNrOfItems=aNrOfRecomendedItems;
	myCurNrOfItems=0;

	myItemList = new Type [myMaxNrOfItems] ();
}



template <class Type,class CountType>
Type& FixedArray<Type,CountType>::operator[](const int& aIndex) const 
{
	assert((aIndex>=0) && (aIndex<myCurNrOfItems) && "OUT OF BOUNDS ERROR");
	return(myItemList[aIndex]);
}

template <class Type,class CountType>
void FixedArray<Type,CountType>::Add(const Type& aItem)
{
	assert(myCurNrOfItems<myMaxNrOfItems && "TRIED To add object into full fixedarray");
	myItemList[myCurNrOfItems]=aItem;
	myCurNrOfItems++;
}


template <class Type,class CountType>
void FixedArray<Type,CountType>::DeleteCyclic(Type& aItem)
{
	For_Count_i(myCurNrOfItems)
	{
		if(myItemList[i]==aItem)
		{
			delete(myItemList[i]);
			myItemList[i]=myItemList[myCurNrOfItems-1];
			myCurNrOfItems--;
			return;
		}
	}
}

template <class Type,class CountType>
void FixedArray<Type,CountType>::DeleteCyclicAtIndex(int aItemNumber)
{
	delete(myItemList[aItemNumber]);
	myItemList[aItemNumber]=myItemList[myCurNrOfItems-1];
	myCurNrOfItems--;
}

template <class Type,class CountType>
void FixedArray<Type,CountType>::RemoveCyclic(const Type& aItem) 
{
	For_Count_i(myCurNrOfItems)
	{
		if(myItemList[i]==aItem)
		{
			myItemList[i]=myItemList[myCurNrOfItems-1];
			myCurNrOfItems--;
			return;
		}
	}
}

template <class Type,class CountType>
void FixedArray<Type,CountType>::RemoveCyclicAtIndex(int aItemNumber)
{
	myItemList[aItemNumber]=myItemList[myCurNrOfItems-1];
	myCurNrOfItems--;
}

template <class Type,class CountType>
void FixedArray<Type,CountType>::Insert(int aIndex,Type& aObject)
{
	assert(myCurNrOfItems<myMaxNrOfItems && "TRIED To insert object into full fixedarray");
	for(int i=myCurNrOfItems-1;i>=aIndex;i--)
	{
		myItemList[i+1]=myItemList[i];
	}
	myItemList[aIndex]=aObject;
	myCurNrOfItems++;
}

template <class Type,class CountType>
void FixedArray<Type,CountType>::RemoveAll()
{
	myCurNrOfItems=0;
}

template <class Type,class CountType>
void FixedArray<Type,CountType>::DeleteAll()
{
	For_Count_i(myCurNrOfItems)
	{
		SAFE_DELETE(myItemList[i]);
	}
	myCurNrOfItems=0;

}

template <class Type,class CountType>
int FixedArray<Type,CountType>::Count() const
{
	return(myCurNrOfItems);
}

template<class Type, class CountType>
void* FixedArray<Type,CountType>::GetArray() const
{
	return myItemList;
}

template<class Type, class CountType>
void FixedArray<Type,CountType>::SetCurrentNrOfItems(int aNumber)
{
	myCurNrOfItems = aNumber;
}

#endif