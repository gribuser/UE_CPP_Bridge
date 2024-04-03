#pragma once
#include "UE_CPP_Bridge_Setup.h"
#include "UE_BridgeBaseTypes.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1

//#include <map>
#include <concepts>
#include <type_traits>
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseMapInfo.h"

//static_assert(0, "Not implemented yet");
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "Containers/Map.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's TMap functionality with llvm::*
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1


template<typename KeyT, typename ValueT>
class P2P_Pair : public std::pair<KeyT, ValueT> {
public:
	//typedef typename std::pair<KeyT, ValueT> Super;
	using Super = std::pair<KeyT, ValueT>;
	//KeyT& Key = Super::first;
	//ValueT& Value = Super::second;
	using Super::pair;

	const KeyT& GetKey() const { return Super::first; }
	ValueT& GetVal() { return Super::second; }
	const ValueT& GetVal() const { return Super::second; }

	// DenseMap uses this methods
	KeyT& getFirst() { return Super::first; }
	const KeyT& getFirst() const { return Super::first; }
	ValueT& getSecond() { return Super::second; }
	const ValueT& getSecond() const { return Super::second; }

};

namespace UE_CPP_Bridge {

template<typename T>
struct DenseMapInfo {
	//static inline T getEmptyKey();
	//static inline T getTombstoneKey();
	//static unsigned getHashValue(const T &Val);
	//static bool isEqual(const T &LHS, const T &RHS);
};

template<class T>
requires(std::is_class_v<T>)
struct DenseMapInfo<T> {
	//using llvm::DenseMapInfo<T, T>::DenseMapInfo;
	static inline T getEmptyKey() {
		if constexpr (requires { T::getEmptyKey(); }) {
			return T::getEmptyKey();
		} else {
			return T();
		}
	}
	static inline T getTombstoneKey() {
		return T::getTombstone();
	}
	static unsigned getHashValue(const T& Val) {
		return Val.GetTypeHash();
	}
	static bool isEqual(const T& LHS, const T& RHS) { return LHS == RHS; }
};

template <typename T> requires(std::is_same_v<T, unsigned int>)
struct DenseMapInfo<T>: llvm::DenseMapInfo<unsigned int> {};

template <typename T> requires(std::is_same_v<T, unsigned short>)
struct DenseMapInfo<T>: llvm::DenseMapInfo<unsigned short> {};

template <typename T> requires(std::is_same_v<T, unsigned long>)
struct DenseMapInfo<T>: llvm::DenseMapInfo<unsigned long> {};

}; // namespace UE_CPP_Bridge

//llvm::DenseMap<typename KeyT, typename ValueT,
//	typename KeyInfoT = DenseMapInfo<KeyT>,
//	typename BucketT = detail::DenseMapPair<KeyT, ValueT>>


template<typename KeyT, typename ValueT,
	typename KeyInfoT = UE_CPP_Bridge::DenseMapInfo<KeyT>,
	typename BucketT = P2P_Pair<KeyT, ValueT>
>
class TMap: public llvm::DenseMap<KeyT, ValueT, KeyInfoT, BucketT> {
public:
	using DenseMapT = llvm::DenseMap<KeyT, ValueT, KeyInfoT, BucketT>;
	using DenseMapT::DenseMap;

	//TMap() = default;
	//TMap(std::initializer_list<TPair<const KeyT&, const ValueT&>> Vals) {
	//	this->init(Vals.size());
	//	this->insert(Vals.begin(), Vals.end());
	//}

	bool Contains(const KeyT& Key) {
		return DenseMapT::contains(Key);
	}

	ValueT& Add(KeyT&& Key, ValueT&& InVal) {
		return Emplace(Key, InVal);
	}
	ValueT& Add(const KeyT& Key, ValueT&& InVal) {
		auto Pair = DenseMapT::try_emplace(Key, InVal);
		if (!Pair.second)
			Pair.first.getSecond() = std::move(InVal);
		return Pair.first.getSecond();
	}

	ValueT& Emplace(KeyT&& Key, ValueT&& InVal) {
		auto Pair = DenseMapT::try_emplace(Key, InVal);
		if (!Pair.second)
			Pair.first.getSecond() = std::move(InVal);
		return Pair.first.getSecond();
	}

	ValueT* Find(const KeyT& Key) {
		//return DenseMapT::find(Key);
		auto Iter = this->find(Key);
		if (Iter != this->end()) {
			return Iter->second;
		}
		return nullptr;
	}
	const ValueT* Find(const KeyT& Key) const {
		auto Iter = this->find(Key);
		if (Iter != this->end()) {
			return Iter->second;
		}
		return nullptr;
	}
};
#endif
