#include "UE_BridgeMisc.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1


int32 FMath::RandRange(int32 Min, int32 Max) {
	thread_local static std::mt19937 mt(std::random_device{}());
	thread_local static std::uniform_int_distribution<int32> pick;
	return pick(mt, decltype(pick)::param_type{Min, Max});
}

#endif
