#pragma once
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 0
#include <vector>
#include <random>
#include <algorithm>
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
#include "CoreMinimal.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

namespace UE_CPP_Bridge {

template<typename InKeyType, typename InElementType>
class TBridgeMap: public TMap<InKeyType, InElementType> {
public:
	using TMap<InKeyType, InElementType>::TMap;
	//InElementType& operator[](InKeyType idx)             { return TMap<InKeyType, InElementType>::TMap[idx]; }
	//const InElementType& operator[](InKeyType idx) const { return TMap<InKeyType, InElementType>::TMap[idx]; }
};
};