#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
static_assert(0, "Not implemented yet");
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "Containers/Set.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's TMap functionality with std::*
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
template<typename InElementType>
class TMap: public std::set<InElementType> {
public:
	using TSet<InElementType>::TSet;
};
#endif
