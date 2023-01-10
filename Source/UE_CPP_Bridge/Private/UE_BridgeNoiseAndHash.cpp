#pragma once
#include "UE_BridgeNoiseAndHash.h"

namespace UE_CPP_Bridge {
const float K1 = (sqrt(3) - 1) / 2;
const float K2 = (3 - sqrt(3)) / 6;

//const int64 K1_Int = 23987; // sqrt(3) = 1.73205080757
//const int64 K2_Int = 13849;// (3 - sqrt(3)) / 3;


// https://rextester.com/VBA54659
// Returns (approximation of) 32768*cos(x*(pi/32768)).
//   x is in units of pi/32768, so 65536 is 2*pi (I. e. period).
//   return value is in [-32768; +32768].
//   max. abs. error: ~3*10^-4 (10 ulp).
//   Cos_Int(0)    =+32768
//   Cos_Int(32768)=-32768
//   approximation is not monotonic on [0; 32768].
int32_t Cos_Int(int64_t x) {
	return Cos_Int((int32_t)x);
}

int32_t Cos_Int(int32_t x) {
	int32_t n = x & 32768;
	x &= 32767;
	x = 32768 - x - x;
	int64_t x2 = (x * x) >> 15;
	int64_t t;
	t = ((x2 * 18984) >> 18) - 21056;
	t = ((x2 * t) >> 15) + 51451;
	x = (x * t) >> 15;
	return n ? -x : x;
}
//
//// -0xFFFF .. 0xFFFF
//FInt64Vector2 SimplexHash_Int(FInt64Vector2 A) {
//	FInt64Vector2 B = {
//		A.X * 4164813 + A.Y * 10213786,
//		A.X * 8830976 + A.Y * 6006374 };
//
//	FInt64Vector2 CS = {
//		Cos_Int(B.X / 205881),
//		Cos_Int(B.Y / 205881) };
//
//	// optimize me later: conls replace % with & here, but needs tuning
//	FInt64Vector2 C = {
//		(((CS.X * 5735432534) >> 16) % 0xFFFF) - 0x7FFF,
//		(((CS.Y * 5735432534) >> 16) % 0xFFFF) - 0x7FFF };
//
//	return C;
//}
//
//void DEBUG_MixtureHash(FVector2D p, FInt64Vector2 A) {
//	FVector2D b = {
//		p.Dot({127.1, 311.7}),
//		p.Dot({269.5, 183.3}) };
//
//	FVector2D cs = { cos(b.X) , cos(b.Y) };
//	auto Fake = cs.X;
//	FVector2D c = {
//		modf(cs.X * 43758.5453123, &Fake),
//		modf(cs.Y * 43758.5453123, &Fake) };
//
//	FInt64Vector2 B = {
//		A.X * 4164813 + A.Y * 10213786,
//		A.X * 8830976 + A.Y * 6006374 };
//
//	FInt64Vector2 CS = {
//		Cos_Int(B.X / 205881),
//		Cos_Int(B.Y / 205881) };
//	FInt64Vector2 C = {
//		(CS.X * 5735432534 >> 16) % 0xFFFF,
//		(CS.Y * 5735432534 >> 16) % 0xFFFF };
//
//}

//// 0 .. 0xFFFF
//int SimplexNoise_Int(FInt64Vector2 P) {
//	P = P & 0xFFFFF;
//	// debug
//	FVector2D p = { (double)P.X, (double)P.Y };
//	FVector2D i = (p + (p.X + p.Y) * K1);
//	i.X = floor(i.X);
//	i.Y = floor(i.Y);
//	FVector2D a = p - i + (i.X + i.Y) * K2;
//	double m = (a.X < a.Y) ? 0.0 : 1.0;
//	FVector2D o(m, 1.0 - m);
//	FVector2D b = a - o + K2;
//	FVector2D c = a - 1.0 + 2.0 * K2;
//	FVector h = -1 * FVector(a.Dot(a), b.Dot(b), c.Dot(c)) + 0.5;
//	if (h.X < 0) { h.X = 0.0; }
//	if (h.Y < 0) { h.Y = 0.0; }
//	if (h.Z < 0) { h.Z = 0.0; }
//
//	FVector n = h * h * h * h * FVector(
//		a.Dot(simplex_hash(i + 0.0)),
//		b.Dot(simplex_hash(i + o)),
//		c.Dot(simplex_hash(i + 1.0)));
//	//debug end
//	P = P << 16; // sorta add decimal digits
//	FInt64Vector2 I = (P + (((P.X + P.Y) * K1_Int) >> 16)) & (~0xFFFF); // sorta round
//
//	FInt64Vector2 A = P - I + (((I.X + I.Y) * K2_Int) / 0x10000);
//
//	int M = (A.X < A.Y) ? 0 : 0xFFFF;
//
//	FInt64Vector2 O(M, 0xFFFF - M);
//	FInt64Vector2 B = A - O + K2_Int;
//	FInt64Vector2 C = A - 0xFFFF + 2 * K2_Int;
//	int64 H1 = -(A.Dot(A) / 0x10000) + 0x7FFF;
//	int64 H2 = -(B.Dot(B) / 0x10000) + 0x7FFF;
//	int64 H3 = -(C.Dot(C) / 0x10000) + 0x7FFF;
//	if (H1 < 0) { H1 = 0; }
//	if (H2 < 0) { H2 = 0; }
//	if (H3 < 0) { H3 = 0; }
//
//	int64 N1 = ((H1 * H1 * H1 * H1) >> 32) * A.Dot(SimplexHash_Int(I));
//	int64 N2 = ((H2 * H2 * H2 * H2) >> 32) * B.Dot(SimplexHash_Int(I + O));
//	int64 N3 = ((H3 * H3 * H3 * H3) >> 32) * C.Dot(SimplexHash_Int(I + 0xFFFF));
//
//	return (N1 >> 32) * 70 + (N2 >> 32) * 70 + (N3 >> 32) * 70;
////	return ((N1 * 70) << 16) + ((N2 * 70) << 16) + ((N3 * 70) << 16);
//}

//// 0..0xFFFF
//int SimplexNoise4Ph_Int(FInt64Vector2 p, uint32 Phase) {
//	p = p + 731 * Phase;
//	FInt64Vector2 m(0x15FFE, 0x11FFE);
//
//	int dx = SimplexNoise_Int(p);
//	p = (m * p) >> 16;
//	dx += SimplexNoise_Int(p) >> 1;
//	p = (m * p) >> 16;
//	dx += SimplexNoise_Int(p) >> 2;
//	p = (m * p) >> 16;
//	dx += SimplexNoise_Int(p) >> 3;
//	return dx;
//}

FVector2D simplex_hash(FVector2D p) {
	p = {
		p.Dot({127.1, 311.7}),
		p.Dot({269.5, 183.3})
	};
	auto IP = p.X;
	p.X = modf(cos(p.X) * 43758.5453123, &IP);
	p.Y = modf(cos(p.Y) * 43758.5453123, &IP);
	return p * 2 - 1.0;
}

template <typename T>
T simplex_noise(FVector2D p) {

	FVector2D i = (p + (p.X + p.Y) * K1);
	i.X = floor(i.X);
	i.Y = floor(i.Y);
	FVector2D a = p - i + (i.X + i.Y) * K2;
	T m = (a.X < a.Y) ? 0.0 : 1.0;
	FVector2D o(m, 1.0 - m);
	FVector2D b = a - o + K2;
	FVector2D c = a - 1.0 + 2.0 * K2;
	FVector h = -1 * FVector(a.Dot(a), b.Dot(b), c.Dot(c)) + 0.5;
	if (h.X < 0) { h.X = 0.0; }
	if (h.Y < 0) { h.Y = 0.0; }
	if (h.Z < 0) { h.Z = 0.0; }

	FVector n = h * h * h * h * FVector(
		a.Dot(simplex_hash(i + 0.0)),
		b.Dot(simplex_hash(i + o)),
		c.Dot(simplex_hash(i + 1.0)));
	return n.Dot({ 70.0,70.0,70.0 });
}

template <typename TOut, typename Tin>
TOut SimplexNoise4Ph(FVector2D p, Tin Phase) {
	p = p + 7.31 * Phase;
	FVector2D m(1.6, 1.2);

	TOut dx = 0.5000 * simplex_noise<TOut>(p); p = m * p;
	dx += 0.2500 * simplex_noise<TOut>(p); p = m * p;
	dx += 0.1250 * simplex_noise<TOut>(p); p = m * p;
	dx += 0.0625 * simplex_noise<TOut>(p);
	return dx;
}

// Copy-paste from https://www.shadertoy.com/view/XlGcRh
// BBS-inspired hash
//  - Olano, Modified Noise for Evaluation on Graphics Hardware, GH 2005
uint32 BBSHash(uint32 v) {
	v = v % 65521u;
	v = (v * v) % 65521u;
	v = (v * v) % 65521u;
	return v;
}

uint32 BBSHash(FIntVector2 v) {
	return BBSHash((uint32)v.X ^ (uint32)v.Y);
}
float BBSHashF(FIntVector2 v) {
	return (float)BBSHash(v) / 0xFFFF;
}


uint32 FastHash(uint32 x) {
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	return (x >> 16) ^ x;
}

uint32 FastHash16(uint32 x) {
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	return ((x >> 16) ^ x) & 0xFFFF;
}


};