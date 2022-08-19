#include "CoreMinimal.h"

namespace UE_CPP_Bridge {
template <typename T>
UE_CPP_BRIDGE_API T simplex_noise(FVector2D P);

template <typename TOut, typename Tin>
UE_CPP_BRIDGE_API TOut SimplexNoise4Ph(FVector2D p, Tin Phase);

UE_CPP_BRIDGE_API uint32 BBSHash(uint32 v);

uint32 BBSHash(FIntVector2 v) {
	return BBSHash(BBSHash((uint32)v.X) + (uint32)v.Y);
}

// 0..1
float BBSHashF(FIntVector2 v) {
	return (float)BBSHash(v) / 0x7FFF;
}

// Returns a random normalized vector based on input
FVector2D BBSHash2D(FIntVector2 v) {
	int32 h32 = BBSHash(v);
	return {
		(float)(h32 & 0xFF) / 0xFF,
		(float)(h32 >> 8) / 0xFF
	};
}

FVector2D BBSHash2D(uint32 v) {
	int32 h32 = BBSHash(v);
	return {
		(float)(h32 & 0xFF) / 0xFF,
		(float)(h32 >> 8) / 0xFF
	};
}

template <typename T>
T PerlinNoise5ph(FVector2D Coord, T Phase);

template UE_CPP_BRIDGE_API float SimplexNoise4Ph(FVector2D p, float Phase);
template UE_CPP_BRIDGE_API float SimplexNoise4Ph<float, uint32>(FVector2D p, uint32 Phase);
template UE_CPP_BRIDGE_API double SimplexNoise4Ph<double, uint32>(FVector2D p, uint32 Phase);
template UE_CPP_BRIDGE_API double SimplexNoise4Ph<double, double>(FVector2D p, double Phase);
template UE_CPP_BRIDGE_API float simplex_noise<float>(FVector2D P);
template UE_CPP_BRIDGE_API double simplex_noise<double>(FVector2D P);
template UE_CPP_BRIDGE_API float PerlinNoise5ph<float>(FVector2D Coord, float Phase);
template UE_CPP_BRIDGE_API double PerlinNoise5ph<double>(FVector2D Coord, double Phase);

};