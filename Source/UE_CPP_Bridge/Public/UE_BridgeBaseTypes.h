#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
#include <cstdint>
#include <chrono>
#include <thread>
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "GenericPlatform/GenericPlatformProcess.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's u/int8,16,32,64 functionality with <cstdint>
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1

//~ Unsigned base types

// 8-bit unsigned integer
typedef unsigned char				uint8;
//typedef uint8_t uint8;

// 16-bit unsigned integer
typedef unsigned short			uint16;

// 32-bit unsigned integer
typedef unsigned int				uint32;

// 64-bit unsigned integer
typedef unsigned long long	uint64;

//~ Signed base types.

// 8-bit signed integer
typedef	signed char			int8;

// 16-bit signed integer
typedef short						int16;

// 32-bit signed integer
typedef int							int32;

// 64-bit signed integer
typedef long long				int64;

//~ Character types

// An ANSI character. 8-bit fixed-width representation of 7-bit characters.
typedef char				ANSICHAR;

// A wide character. In-memory only. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms.
typedef wchar_t				WIDECHAR;

// An 8-bit character type. In-memory only. 8-bit representation. Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++20 yet.
enum UTF8CHAR: unsigned char {};

// An 8-bit character type. In-memory only. 8-bit representation.
/* UE_DEPRECATED(5.0) */ [[deprecated("FPlatformTypes::CHAR8 is deprecated, please use FPlatformTypes::UTF8CHAR instead.")]]
typedef uint8				CHAR8;

// A 16-bit character type. In-memory only.  16-bit representation. Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef uint16				CHAR16;

// A 32-bit character type. In-memory only. 32-bit representation. Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef uint32				CHAR32;

// already defined in winnt.h
//// A switchable character. In-memory only. Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.
//typedef WIDECHAR			TCHAR;


/* Numeric constants
 *****************************************************************************/

#define MIN_uint8		((uint8)	0x00)
#define	MIN_uint16		((uint16)	0x0000)
#define	MIN_uint32		((uint32)	0x00000000)
#define MIN_uint64		((uint64)	0x0000000000000000)
#define MIN_int8		((int8)		-128)
#define MIN_int16		((int16)	-32768)
#define MIN_int32		((int32)	0x80000000)
#define MIN_int64		((int64)	0x8000000000000000)

#define MAX_uint8		((uint8)	0xff)
#define MAX_uint16		((uint16)	0xffff)
#define MAX_uint32		((uint32)	0xffffffff)
#define MAX_uint64		((uint64)	0xffffffffffffffff)
#define MAX_int8		((int8)		0x7f)
#define MAX_int16		((int16)	0x7fff)
#define MAX_int32		((int32)	0x7fffffff)
#define MAX_int64		((int64)	0x7fffffffffffffff)

#define MIN_flt			(1.175494351e-38F)			/* min positive value */
#define MAX_flt			(3.402823466e+38F)
#define MIN_dbl			(2.2250738585072014e-308)	/* min positive value */
#define MAX_dbl			(1.7976931348623158e+308)	


enum { INDEX_NONE = -1 };



struct FPlatformProcess {
  void static Sleep(const double ASeconds) {
    std::this_thread::sleep_for(std::chrono::duration<double>(ASeconds));
  }
};

struct FVector2D {
public:
	//using FReal = T;

	/** Vector's X component. */
	double X;

	/** Vector's Y component. */
	double Y;

	FVector2D(double InX, double InY)
		: X(InX), Y(InY)
	{ }

	double Dot(const FVector2D& V2) const {
		return X * V2.X + Y * V2.Y;
	}
	FVector2D operator*(double Scale) const {
		return FVector2D(X * Scale, Y * Scale);
	}
	FVector2D operator/(double Scale) const {
		const double RScale = 1.f / Scale;
		return FVector2D(X * RScale, Y * RScale);
	}
	FVector2D operator+(double A) const {
		return FVector2D(X + A, Y + A);
	}
	FVector2D operator-(double A) const {
		return FVector2D(X - A, Y - A);
	}
	FVector2D operator*(const FVector2D& V2) const {
		return FVector2D(X * V2.X, Y * V2.Y);
	}
	FVector2D operator/(const FVector2D& V2) const {
		return FVector2D(X / V2.X, Y / V2.Y);
	}
	FVector2D operator+(const FVector2D& V2) const {
		return FVector2D(X + V2.X, Y + V2.Y);
	}
	FVector2D operator-(const FVector2D& V2) const {
		return FVector2D(X - V2.X, Y - V2.Y);
	}
};

struct FVector {
public:
	double X;
	double Y;
	double Z;

	FVector(double InX, double InY, double InZ = 0.0)
		: X(InX), Y(InY), Z(InZ)
	{ }

	double Dot(const FVector& V2) const {
		return X * V2.X + Y * V2.Y + Z * V2.Z;
	}
	FVector operator*(double Scale) const {
		return FVector(X * Scale, Y * Scale, Z * Scale);
	}
	FVector operator/(double Scale) const {
		const double RScale = 1.f / Scale;
		return FVector(X * RScale, Y * RScale, Z * RScale);
	}
	FVector operator+(double A) const {
		return FVector(X + A, Y + A, Z + A);
	}
	FVector operator-(double A) const {
		return FVector(X - A, Y - A, Z - A);
	}
	FVector operator*(const FVector& V) const {
		return FVector(X * V.X, Y * V.Y, Z * V.Z);
	}
	FVector operator/(const FVector& V) const {
		return FVector(X / V.X, Y / V.Y, Z / V.Z);
	}
	FVector operator+(const FVector& V) const {
		return FVector(X + V.X, Y + V.Y, Z + V.Z);
	}
	FVector operator-(const FVector& V) const {
		return FVector(X - V.X, Y - V.Y, Z - V.Z);
	}
};

struct FIntVector2
{
public:
	//using FReal = T;

	/** Vector's X component. */
	int64 X;

	/** Vector's Y component. */
	int64 Y;
};

#endif
