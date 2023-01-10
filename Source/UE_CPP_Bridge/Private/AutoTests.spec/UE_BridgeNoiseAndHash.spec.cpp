#pragma once

#include "UE_BridgeNoiseAndHash.h"

using namespace UE_CPP_Bridge;

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(UE_CPP_Bridge_NoiseAndHashes,
									"UE_CPP_Bridge.NoiseAndHashes",
									EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	bool B = true;
END_DEFINE_SPEC(UE_CPP_Bridge_NoiseAndHashes)

void UE_CPP_Bridge_NoiseAndHashes::Define() {
Describe("Simplex noise over integer", [this]() {
	It("Cos_Int should behave somewhat like cos", [this]() {
		TArray<float> ValuesToCheck1 = {	0.0, 3.14159265359, 0.01, 0.1, 0.45, 0.5, 0.75, 0.891, 0.98, 1.0 };
		TArray<float> ValuesToCheck2 = { 0.0, 0.5, 1.0, 1.45, 99.0, 96.75 };
		for (float Val1 : ValuesToCheck1) {
			for (float Val2 : ValuesToCheck2) {
				for (float Val3 : ValuesToCheck2) {
					float fVal = Val1 * Val2 + Val3;
					int64_t iVal = fVal * 32768 / PI;
					float CosTrue = cos(fVal);
					int32_t CosInt = Cos_Int(iVal);
					float CosInt_AsFloat = (float)CosInt / 32768;
					B = TestTrue(FString::Printf(TEXT("Cos %1.4f calc ok"), fVal), abs(CosInt_AsFloat - CosTrue) < 0.01);
				}
			}
		}

	});
	//It("SimplexHash_Int should behave somewhat like simplex_hash", [this]() {
	//	TArray<float> ValuesToCheck1 = { 0.0, 1.0, 3.14159265359, 0.01, 10.1, 64.5, 1886952};
	//	TArray<float> ValuesToCheck2 = { 0.0, 0.5, 1.0, 1.45, 99.0, 96.75 };
	//	float PrevVal = 0.0;
	//	for (float Val1 : ValuesToCheck1) {
	//		for (float Val2 : ValuesToCheck2) {
	//			for (float Val3 : ValuesToCheck2) {
	//				float fVal = Val1 * Val2 + Val3;

	//				FVector2D HashF = { fVal , PrevVal };
	//				FInt64Vector2 HashI = {
	//					(int64)((int64)0xFFFF * HashF.X),
	//					(int64)((int64)0xFFFF * HashF.Y) };

	//				FVector2D SimplexHashF = simplex_hash(HashF);
	//				FInt64Vector2 SimplexHashI = SimplexHash_Int(HashI);

	//				DEBUG_MixtureHash(HashF, HashI);

	//				FVector2D SimplexHashI_AsFloat = {
	//					(double)SimplexHashI.X / 0x7FFF,
	//					(double)SimplexHashI.Y / 0x7FFF
	//				};

	//				B = TestTrue(FString::Printf(TEXT("Hash for %1.4f:%1.4f calc ok"), HashF.X, HashF.Y),
	//										 abs(SimplexHashI_AsFloat.X - SimplexHashF.X) < 0.01
	//										 && abs(SimplexHashI_AsFloat.Y - SimplexHashF.Y) < 0.01);

	//				PrevVal = fVal;
	//			}
	//		}
	//	}
	//});
});
}

#endif