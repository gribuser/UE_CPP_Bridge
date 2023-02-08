#pragma once
#include "CoreMinimal.h"

namespace UE_CPP_Bridge {
template <typename T>
UE_CPP_BRIDGE_API T simplex_noise(FVector2D P);

template <typename TOut, typename Tin>
UE_CPP_BRIDGE_API TOut SimplexNoise4Ph(FVector2D p, Tin Phase);

UE_CPP_BRIDGE_API uint32 BBSHash(uint32 v);
UE_CPP_BRIDGE_API uint32 FastHash(uint32 v);
UE_CPP_BRIDGE_API uint32 FastHash16(uint32 v);
UE_CPP_BRIDGE_API uint32 BBSHash(FIntVector2 v);

// 0..1
UE_CPP_BRIDGE_API float BBSHashF(FIntVector2 v);

// Returns a random normalized vector based on input
inline FIntVector2 BBSHash2D(FIntVector2 v) {
	int32 h32 = BBSHash(v);
	return {
		h32 & 0xFF,
		h32 >> 8 
	};
}

inline FIntVector2 BBSHash2D(uint32 v) {
	int32 h32 = BBSHash(v);
	return {
		h32 & 0xFF,
		h32 >> 8
	};
}

struct FInt64Vector2 {
	int64 X;
	int64 Y;
	FInt64Vector2 operator + (const FInt64Vector2& A) const { return { X + A.X, Y + A.Y }; }
	FInt64Vector2 operator - (const FInt64Vector2& A) const { return { X - A.X, Y - A.Y }; }
	FInt64Vector2 operator * (const FInt64Vector2& A) const { return { X * A.X, Y * A.Y }; }
	FInt64Vector2 operator / (const FInt64Vector2& A) const { return { X / A.X, Y / A.Y }; }
	FInt64Vector2 operator + (const int64 A) const { return { X + A, Y + A }; }
	FInt64Vector2 operator - (const int64 A) const { return { X - A, Y - A }; }
	FInt64Vector2 operator * (const int64 A) const { return { X * A, Y * A }; }
	FInt64Vector2 operator / (const int64 A) const { return { X / A, Y / A }; }
	FInt64Vector2 operator & (const int64 A) const { return { X & A, Y & A }; }
	FInt64Vector2 operator >> (int A) const { return { X >> A, Y >> A }; }
	FInt64Vector2 operator << (int A) const { return { X << A, Y << A }; }
	FInt64Vector2(int64 AX, int64 AY) :X(AX), Y(AY) {}
	FInt64Vector2(int AX, int AY) :X(AX), Y(AY) {}
	FInt64Vector2(const FIntVector2& A) :X(A.X), Y(A.Y) {}
	int64 Dot(const FInt64Vector2& A) const { return X * A.X + Y * A.Y; }
};
UE_CPP_BRIDGE_API int32_t Cos_Int(int64_t x);
UE_CPP_BRIDGE_API int32_t Cos_Int(int32_t x);
//UE_CPP_BRIDGE_API FInt64Vector2 SimplexHash_Int(FInt64Vector2 A);
//UE_CPP_BRIDGE_API FVector2D simplex_hash(FVector2D p);
//UE_CPP_BRIDGE_API int SimplexNoise_Int(FInt64Vector2 P);
//UE_CPP_BRIDGE_API int SimplexNoise4Ph_Int(FInt64Vector2 p, uint32 Phase);

//void DEBUG_MixtureHash(FVector2D p, FInt64Vector2 A);


inline FIntVector2 operator +(FIntVector2& A, int B) {
	return { A.X + B, A.Y + B };
}
inline FIntVector2 operator *(FIntVector2& A, int B) {
	return { A.X * B, A.Y * B };
}
inline FIntVector2 operator /(FIntVector2& A, int B) {
	return { A.X / B, A.Y / B };
}
inline FIntVector2 operator & (FIntVector2& A, int B) {
	return { A.X & B, A.Y & B };
}

template UE_CPP_BRIDGE_API float SimplexNoise4Ph(FVector2D p, float Phase);
template UE_CPP_BRIDGE_API float SimplexNoise4Ph<float, uint32>(FVector2D p, uint32 Phase);
template UE_CPP_BRIDGE_API double SimplexNoise4Ph<double, uint32>(FVector2D p, uint32 Phase);
template UE_CPP_BRIDGE_API double SimplexNoise4Ph<double, double>(FVector2D p, double Phase);
template UE_CPP_BRIDGE_API float simplex_noise<float>(FVector2D P);
template UE_CPP_BRIDGE_API double simplex_noise<double>(FVector2D P);

};