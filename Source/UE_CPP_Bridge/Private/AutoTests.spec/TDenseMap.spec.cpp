#pragma once
#if WITH_DEV_AUTOMATION_TESTS
#include "TDenseMap.h"
using namespace UE_CPP_Bridge;
//#include "SpecTestsShared.h"


BEGIN_DEFINE_SPEC(UE_CPP_Bridge_TDenseMap,
	"UE_CPP_Bridge.TDenseMap",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
	bool B;

	template<typename KeyT, typename ValueT>
	void AddTestsForTypes(const FString& TypeName, TArray<KeyT> keys, TArray<ValueT> values){
		It(TypeName, [this, keys, values]() {
			TDenseMap<KeyT, ValueT> Map = {
				{keys[0], values[0]},
			};

			TestTrue("Map is not empty", !Map.IsEmpty());
			TestEqual("check count elements Num()", Map.Num(), 1);
			TestTrue("key in Map: Contains()", Map.Contains(keys[0]));
			TestEqual("check Find()", *Map.Find(keys[0]), values[0]);
			int i = 0;
			for (auto& Pair : Map) {
				TestEqual("Pair.GetKey() is ok", Pair.GetKey(), keys[0]);
				TestEqual("Pair.GetVal() is ok", Pair.GetVal(), values[0]);
				i++;
			}
			B = TestEqual("iteration count", i, 1);
			Map.Add(keys[1], values[1]);
			Map.Emplace(keys[2]);
			TestEqual("elements count Num()", Map.Num(), 3);
			TestTrue("key0 still there", Map.Contains(keys[0]));
			TArray<KeyT> Keys;
			Map.GetKeys(Keys);
			TestEqual("GetKeys() is ok", Keys.Num(), 3);

			Map.Remove(keys[0]);
			TestEqual("after remove element count Num()", Map.Num(), 2);
			TestFalse("removed key not in Map", Map.Contains(keys[0]));
			ValueT* val1 = Map.Find(keys[1]);
			TestNotNull("found val1 by key1", val1);
			Map.Reset();
			TestTrue("Map is empty after Reset()", Map.IsEmpty());
			Map.Append({{keys[0], values[0]}});
			TestTrue("key0 is there again", Map.Contains(keys[0]));
		});
	}

END_DEFINE_SPEC(UE_CPP_Bridge_TDenseMap)

#define REGISTER_DENSE_MAP_TEST(KeyT, ValueT, TestKeys, TestValues)\
	AddTestsForTypes<KeyT, ValueT>(TEXT(#KeyT) TEXT(" -> ") TEXT(#ValueT), TArray<KeyT>TestKeys, TArray<ValueT>TestValues)

void UE_CPP_Bridge_TDenseMap::Define() {
	BeforeEach([this]() {
	});
	AfterEach([this]() {
	});

	Describe("Types key and value", [this]() {
		REGISTER_DENSE_MAP_TEST(int, float, ({100500, 42, 128}), ({42.32, 0.2, 100500.500100}));
		REGISTER_DENSE_MAP_TEST(uint32, FString, ({100500u, 1, 2}), ({"Ели", "мясо", "мужики"}));
		REGISTER_DENSE_MAP_TEST(FString, int,
			({"пивом", "запивали", "!!!"}),
			({11, 222, 3333})
		);
		REGISTER_DENSE_MAP_TEST(FName, int32,
			({FName("A"), FName("B"), FName("C")}),
			({999, 1024, -42})
		);
	});
}
#endif
