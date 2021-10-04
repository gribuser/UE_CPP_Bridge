#pragma once
#include "EU_CPP_BridgeMisc.h"


namespace PrimitiveGames{ namespace Utils {

uint32 UE_CPP_BRIDGE_API CalcCrc32(const void* InData, int32 Length, uint32 CRC) {
	return FCrc::MemCrc32(InData, Length, CRC);
}

}}