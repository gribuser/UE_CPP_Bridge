#pragma once
#include "UE_CPP_Bridge_Setup.h"
#include "UE_BridgeLocking.h"
#include "UE_BridgeArray.h"
#include "UE_BridgeMap.h"
#include "UE_BridgeSet.h"

namespace UE_CPP_Bridge {

// Enchanced with locking/unlocking functions array
template<typename InElementType>
class TThreadedArray: public TArray<InElementType>, public FThreadsafeReadable {
public:
	using TArray<InElementType>::TArray;
	bool ThreadSafeContains(const InElementType& Item) const {
		BeginRead();
		const bool Out = this->Contains(Item);
		EndRead();
		return Out;
	}
	size_t ThreadSafeAdd(const InElementType& Item) {
		BeginWrite();
		const size_t Out = this->Add(Item);
		EndWrite();
		return Out;
	}
	void ThreadSafeRemoveAtSwap(size_t Index) {
		BeginWrite();
		this->RemoveAtSwap(Index);
		EndWrite();
	}
	size_t SubstituteOrAdd(const InElementType& Item, const InElementType& Replaceable = NULL) {
		UE_CPP_BRIDGE_DEV_TRAP(IsLocked());
		for (size_t i = 0; i < this->Num(); i++) {
			if ((*this)[i] == Replaceable) {
				(*this)[i] = Item;
				return i;
			}
		}
		return this->Add(Item);
	}
	bool bIsReallyEmpty( const InElementType& Empty = NULL){
		for (size_t i = 0; i < this->Num(); i++) {
			if ((*this)[i] != Empty) { return false; }
		}
		return true;
	}
	operator TArray<InElementType>() const { return static_cast<TArray<InElementType>>(*this); }
	TThreadedArray<InElementType>& operator =(const TArray<InElementType>& A) {
		TArray<InElementType>::Reset();
		TArray<InElementType>::Append(A);
//		static_cast<TArray<InElementType>>(*this) = A;
		return *this;
	}
};

template<typename InKeyType, typename InElementType>
class TThreadedMap: public TMap<InKeyType, InElementType>, public FThreadsafeReadable {
public:
	using TMap<InKeyType, InElementType>::TMap;
	bool ThreadSafeContains(const InKeyType& Item) const {
		BeginRead();
		bool Out = this->Contains(Item);
		EndRead();
		return Out;
	}
	InElementType& ThreadSafeAdd(const InKeyType& Key, const InElementType& Value) {
		BeginWrite();
		InElementType& Out = this->Add(Key, Value);
		EndWrite();
		return Out;
	}
	int32 ThreadSafeRemove(const InKeyType& Item) {
		BeginWrite();
		int32 Out = this->Remove(Item);
		EndWrite();
		return Out;
	}
	InElementType ThreadSafePointerFind(const InKeyType& Item) {
		BeginRead();
		InElementType* OutRaw = this->Find(Item);
		InElementType Out = OutRaw == NULL ? NULL : *OutRaw;
		EndRead();
		return Out;
	}
	const InElementType ThreadSafePointerFind(const InKeyType& Item) const {
		BeginRead();
		const InElementType* OutRaw = this->Find(Item);
		const InElementType Out = OutRaw == NULL ? NULL : *OutRaw;
		EndRead();
		return Out;
	}
};

template<typename InElementType>
class TThreadedSet: public TSet<InElementType>, public FThreadsafeReadable {
public:
	using TSet<InElementType>::TSet;
	bool ThreadSafeContains(const InElementType& Item) const {
		BeginRead();
		bool Out = Contains(Item);
		EndRead();
		return Out;
	}
};

};