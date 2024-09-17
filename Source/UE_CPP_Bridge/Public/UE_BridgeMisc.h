#pragma once
#include "UE_CPP_Bridge_Setup.h"
#include "UE_BridgeBaseTypes.h"

#if UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE == 1
	#include <random>
#include <functional>
#elif UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE == 2
//#include "Math/UnrealMathUtility.h" // << included in CoreMinimal.h
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE description for details")
#endif

#if UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE == 1

class FMath {
public:
	static int32 RandRange(int32 Min, int32 Max);

};

#define TFunction std::function

//class TFunction final: public std::function<FuncType> {
//template <typename FuncType, typename... ParamTypes>
//class TFunction final: public std::function<FuncType (ParamTypes...)> {
//	//FuncType operator()(ParamTypes... Params) const
//	//{
//	//	#if DO_CHECK
//	//	if constexpr (StorageType::bCanBeNull) {
//	//		CheckCallable();
//	//	}
//	//	#endif
//	//	return Callable(Storage.GetPtr(), Params...);
//	//}
//
//};


#endif
