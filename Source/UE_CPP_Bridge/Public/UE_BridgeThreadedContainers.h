#pragma once
#include "UE_CPP_Bridge_Setup.h"
#include "CoreMinimal.h"
#include "UE_BridgeLocking.h"
#include "UE_BridgeArray.h"
#include "UE_BridgeMap.h"
#include "UE_BridgeSet.h"

namespace UE_CPP_Bridge {

// Enchanced with locking/unlocking functions array
template<typename InElementType>
class TThreadedArray: public TArray<InElementType>, public FThreadsafeReadable {
public:
	using TArray<InElementType>::TP2PArray;
	bool ThreadSafeContains(const InElementType& Item) const {
		BeginRead();
		bool Out = Contains(Item);
		EndRead();
		return Out;
	}
	size_t ThreadSafeAdd(const InElementType& Item) {
		BeginWrite();
		size_t Out = Add(Item);
		EndWrite();
		return Out;
	}
};

template<typename InKeyType, typename InElementType>
class TThreadedMap: public TMap<InKeyType, InElementType>, public FThreadsafeReadable {
public:
	using TMap<InKeyType, InElementType>::TP2PMap;
	bool ThreadSafeContains(const InKeyType& Item) const {
		BeginRead();
		bool Out = Contains(Item);
		EndRead();
		return Out;
	}
	InElementType& ThreadSafeAdd(const InKeyType& Key, const InElementType& Value) {
		BeginWrite();
		InElementType& Out = Add(Key, Value);
		EndWrite();
		return Out;
	}
};

template<typename InElementType>
class TThreadedSet: public TSet<InElementType>, public FThreadsafeReadable {
public:
	using TSet<InElementType>::TP2PSet;
	bool ThreadSafeContains(const InElementType& Item) const {
		BeginRead();
		bool Out = Contains(Item);
		EndRead();
		return Out;
	}
};

};