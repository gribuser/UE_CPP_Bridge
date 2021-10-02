#pragma once
#include "CoreMinimal.h"
#include "UE_BridgeLocking.h"
#include "UE_BridgeArray.h"
#include "UE_BridgeMap.h"
#include "UE_BridgeSet.h"

namespace UE_CPP_Bridge {

// Enchanced with locking/unlocking functions array
template<typename InElementType>
class TThreadedArray: public TP2PArray<InElementType>, public FCriticalSectionReadable {
public:
	using TP2PArray<InElementType>::TP2PArray;
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
class TThreadedMap: public TP2PMap<InKeyType, InElementType>, public FCriticalSectionReadable {
public:
	using TP2PMap<InKeyType, InElementType>::TP2PMap;
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
class TThreadedSet: public TP2PSet<InElementType>, public FCriticalSectionReadable {
public:
	using TP2PSet<InElementType>::TP2PSet;
	bool ThreadSafeContains(const InElementType& Item) const {
		BeginRead();
		bool Out = Contains(Item);
		EndRead();
		return Out;
	}
};

};