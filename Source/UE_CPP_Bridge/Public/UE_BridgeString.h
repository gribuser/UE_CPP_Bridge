#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
#include <string>
#include <cstdio>
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "Containers/UnrealString.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's FString functionality with std::string
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1

//template< class ObjectType, ESPMode InMode >
class FString : private std::string {
	//using std::shared_ptr<ObjectType>::shared_ptr;
	//FORCEINLINE TSharedPtr(ObjectType* ObjectPtr) : std::shared_ptr<ObjectType>(ObjectPtr);

/**
 * Constructs FString object similarly to how classic sprintf works.
 *
 * @param Format	Format string that specifies how FString should be built optionally using additional args. Refer to standard printf format.
 * @param ...		Depending on format function may require additional arguments to build output object.
 *
 * @returns FString object that was constructed using format and additional parameters.
 */
	template <typename FmtType, typename... Types>
	static FString Printf(const FmtType& Fmt, Types... Args) {
		//static_assert(TIsArrayOrRefOfType<FmtType, TCHAR>::Value, "Formatting string must be a TCHAR array.");
		//static_assert(TAnd<TIsValidVariadicFunctionArg<Types>...>::Value, "Invalid argument(s) passed to FString::Printf");

		return std::sprintf(Fmt, Args...);
	}

	///**
	// * Format the specified string using the specified arguments. Replaces instances of {0} with indices from the given array matching the index specified in the token
	// * @param InExpression		A string representing the format expression
	// * @param InArgs			An array of ordered arguments that match the tokens specified in InExpression
	// * @return A string containing the formatted text
	// */
	//FString Format(const TCHAR* InExpression, const TArray<FStringFormatArg>& InArgs) const {
	//	auto Result = FormatInternal(InExpression, InArgs, false);
	//	if (ensure(Result.IsValid()))
	//	{
	//		return MoveTemp(Result.GetValue());
	//	}

	//	return InExpression;
	//}
};


#endif
