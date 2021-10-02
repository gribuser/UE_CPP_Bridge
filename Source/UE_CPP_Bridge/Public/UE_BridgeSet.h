#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
static_assert(0, "Not implemented yet");
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "CoreMinimal.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

namespace UE_CPP_Bridge {

template<typename InElementType>
class TBridgeSet: public TSet<InElementType> {
public:
	using TSet<InElementType>::TSet;
};
};