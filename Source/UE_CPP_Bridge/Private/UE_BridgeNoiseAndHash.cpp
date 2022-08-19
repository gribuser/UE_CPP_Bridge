#include "UE_BridgeNoiseAndHash.h"

namespace UE_CPP_Bridge {
const float K1 = (sqrt(3) - 1) / 2;
const float K2 = (3 - sqrt(3)) / 6;

template <typename T>
FVector2D simplex_hash(FVector2D p) {
	p = {
		p.Dot({127.1, 311.7}),
		p.Dot({269.5, 183.3})
	};
	T IP;
	p.X = modf(sin(p.X) * 43758.5453123, &IP);
	p.Y = modf(sin(p.Y) * 43758.5453123, &IP);
	return p * 2 - 1.0;
}

template <typename T>
T simplex_noise(FVector2D P) {

	FVector2D i = (P + (P.X + P.Y) * K1);
	i.X = floor(i.X);
	i.Y = floor(i.Y);
	FVector2D a = P - i + (i.X + i.Y) * K2;
	T m = (a.X < a.Y) ? 0.0 : 1.0;
	FVector2D o(m, 1.0 - m);
	FVector2D b = a - o + K2;
	FVector2D c = a - 1.0 + 2.0 * K2;
	FVector h = -1 * FVector(a.Dot(a), b.Dot(b), c.Dot(c)) + 0.5;
	if (h.X < 0) { h.X = 0.0; }
	if (h.Y < 0) { h.Y = 0.0; }
	if (h.Z < 0) { h.Z = 0.0; }

	FVector n = h * h * h * h * FVector(
		a.Dot(simplex_hash<T>(i + 0.0)),
		b.Dot(simplex_hash<T>(i + o)),
		c.Dot(simplex_hash<T>(i + 1.0)));
	return n.Dot({ 70.0,70.0,70.0 });
}

template <typename TOut, typename Tin>
TOut SimplexNoise4Ph(FVector2D p, Tin Phase) {
	p = p + 7.31 * Phase;
	FVector2D m(1.6, 1.2);

	TOut dx = 0.5000 * simplex_noise<TOut>(p); p = m * p;
	dx += 0.2500 * simplex_noise<TOut>(p); p = m * p;
	dx += 0.1250 * simplex_noise<TOut>(p); p = m * p;
	dx += 0.0625 * simplex_noise<TOut>(p); p = m * p;
	return dx;
}

// Copy-paste from https://www.shadertoy.com/view/XlGcRh
// BBS-inspired hash
//  - Olano, Modified Noise for Evaluation on Graphics Hardware, GH 2005
uint32 BBSHash(uint32 v) {
	v = v % 65521u;
	v = (v * v) % 65521u;
	v = (v * v) % 65521u;
	return v << 16;
}

template <typename T>
T PerlinNoise5ph(FVector2D Coord, T Phase) {
	T v = 0.0f;
	T scale = 0.3f;
	T weight = 1.0f;
	T weightTotal = 0.0f;
	for (int i = 0; i < 5; i++) {
		v += FMath::PerlinNoise2D(Coord * scale) * weight;
		weightTotal += weight;
		scale *= 0.5f;
		weight *= 2.0f;
	}
	return v / weightTotal;
}

};