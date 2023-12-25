#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1

#define _ENFORCE_MATCHING_ALLOCATORS 0
#include <map>

//static_assert(0, "Not implemented yet");
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "Containers/Map.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's TMap functionality with std::*
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1


template<typename InKeyType, typename InElementType>
class P2PPair : private std::pair<InKeyType, InElementType> {
public:
	using std::pair<InKeyType, InElementType>::first;
	using std::pair<InKeyType, InElementType>::second;
	const InKeyType& Key() {
		return first;
	}
	InElementType Value() {
		return second;
	}
};

template<typename InKeyType, typename InElementType>
class TMap: public std::map<
	InKeyType,
	InElementType,
	std::less<InKeyType>,
	std::allocator<P2PPair<const InKeyType, InElementType>>
> {
public:
	using TMap<InKeyType, InElementType>::TMap;
	//InElementType& operator[](InKeyType idx)             { return TMap<InKeyType, InElementType>::TMap[idx]; }
	//const InElementType& operator[](InKeyType idx) const { return TMap<InKeyType, InElementType>::TMap[idx]; }
	bool Contains(const InKeyType& Key) {
		return contains(Key);
	}

	InElementType& Emplace(InKeyType&& Key, InElementType&& InVal) {
		auto Pair = emplace(Key, InVal);
		return Pair.second;
	}

	InElementType* Find(const InKeyType& Key) {
		return find(Key);
	}
	const InElementType* Find(const InKeyType& Key) const {
		return find(Key);
	}
};
#endif
