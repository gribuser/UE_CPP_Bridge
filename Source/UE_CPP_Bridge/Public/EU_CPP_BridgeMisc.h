#pragma once
#include "Misc/Crc.h"

namespace PrimitiveGames{ namespace Utils {

uint32 UE_CPP_BRIDGE_API CalcCrc32(const void* InData, int32 Length, uint32 CRC = 0);

}}