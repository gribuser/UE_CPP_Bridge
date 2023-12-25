#pragma once
#include "UE_CPP_Bridge_Setup.h"
#include "UE_BridgeBaseTypes.h"

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

class FString : private std::string {
public:
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

	/** Get the length of the string, excluding terminating character */
	FORCEINLINE int32 Len() const {
		return (int32)length();
	}

	/**
	 * Get pointer to the string
	 *
	 * @Return Pointer to Array of TCHAR if Num, otherwise the empty string
	 */
	FORCEINLINE const char* operator*() const {
		return data();
	}
};

//using FTCHARToUTF8 /*please use StringCast<UTF8CHAR>(PtrToTChar) instead.")*/ = TStringConversion<UE::Core::Private::FTCHARToUTF8_Convert>;
//using FUTF8ToTCHAR /*please use StringCast<TCHAR>(PtrToUTF8Char) instead.")*/ = TStringConversion<FUTF8ToTCHAR_Convert>;
//
//#define TCHAR_TO_ANSI(str) /*please use StringCast<ANSICHAR>(PtrToTChar) instead.")*/ (ANSICHAR*)StringCast<ANSICHAR>(static_cast<const TCHAR*>(str)).Get()
//#define ANSI_TO_TCHAR(str) /*please use StringCast<TCHAR>(PtrToAnsiChar) instead.")*/ (TCHAR*)StringCast<TCHAR>(static_cast<const ANSICHAR*>(str)).Get()
//#define TCHAR_TO_UTF8(str) /*please use StringCast<UTF8CHAR>(PtrToTChar) instead.")*/ (UTF8CHAR*)StringCast<UTF8CHAR>(PtrToTChar) (ANSICHAR*)FTCHARToUTF8((const TCHAR*)str).Get()
//#define UTF8_TO_TCHAR(str) /*please use StringCast<TCHAR>(PtrToUTF8Char) instead.")*/ (TCHAR*)FUTF8ToTCHAR((const ANSICHAR*)str).Get()

//#define TCHAR_TO_ANSI(str) (ANSICHAR*)StringCast<ANSICHAR>(static_cast<const TCHAR*>(str)).Get()
//#define ANSI_TO_TCHAR(str) (TCHAR*)StringCast<TCHAR>(static_cast<const ANSICHAR*>(str)).Get()
//#define TCHAR_TO_UTF8(str) (ANSICHAR*)FTCHARToUTF8((const TCHAR*)str).Get()
//#define UTF8_TO_TCHAR(str) (TCHAR*)FUTF8ToTCHAR((const ANSICHAR*)str).Get()

//#define TCHAR_TO_UTF8(str) 
#define TCHAR_TO_UTF8

#endif
