#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
#include <map>
static_assert(0, "Not implemented yet");
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "Containers/Map.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's TMap functionality with std::*
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
template<typename InKeyType, typename InElementType>
class TMap: public std::map<InKeyType, InElementType> {
public:
	using TMap<InKeyType, InElementType>::TMap;
	//InElementType& operator[](InKeyType idx)             { return TMap<InKeyType, InElementType>::TMap[idx]; }
	//const InElementType& operator[](InKeyType idx) const { return TMap<InKeyType, InElementType>::TMap[idx]; }
};
#endif
