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

//llvm::DenseMap<typename KeyT, typename ValueT,
//	typename KeyInfoT = DenseMapInfo<KeyT>,
//	typename BucketT = detail::DenseMapPair<KeyT, ValueT>>


template<typename KeyT, typename ValueT,
	typename KeyInfoT = UE_CPP_Bridge::DenseMapInfo<KeyT>,
	typename BucketT = P2P_Pair<KeyT, ValueT>
>
class TDenseMap: public llvm::DenseMap<KeyT, ValueT, KeyInfoT, BucketT> {
	using DenseMapT = llvm::DenseMap<KeyT, ValueT, KeyInfoT, BucketT>;
public:
	using DenseMapT::DenseMap;

	//TDenseMap() = default;
	//TDenseMap(std::initializer_list<TPair<const KeyT&, const ValueT&>> Vals) {
	//	this->init(Vals.size());
	//	this->insert(Vals.begin(), Vals.end());
	//}

	bool Contains(const KeyT& Key) const {
		return this->contains(Key);
	}

	ValueT& Add(KeyT&& Key, ValueT&& InVal) {
		return Emplace(Key, InVal);
	}
	ValueT& Add(const KeyT& Key, const ValueT& InVal) {
		auto Pair = this->try_emplace(Key, InVal);
		if (!Pair.second) Pair.first->getSecond() = InVal;
		return Pair.first->getSecond();
	}
	ValueT& Add(const KeyT& Key, ValueT&& InVal) {
		auto Pair = this->try_emplace(Key, InVal);
		if (!Pair.second)
			Pair.first->getSecond() = std::move(InVal);
		return Pair.first->getSecond();
	}
	ValueT& Add(const KeyT& Key, ValueT& InVal) {
		auto Pair = this->try_emplace(Key, InVal);
		if (!Pair.second) Pair.first.getSecond() = std::move(InVal);
		return Pair.first.getSecond();
	}

	void Append(TDenseMap<KeyT, ValueT>&& OtherMap) {
		this->reserve(this->size() + OtherMap.size());
		for (auto& Pair : OtherMap) {
			this->Add(std::move(Pair.Key), std::move(Pair.Value));
		}

		OtherMap.clear();
	}
	void Append(const TDenseMap<KeyT, ValueT>& OtherMap) {
		this->reserve(this->size() + OtherMap.size());
		for (auto& Pair : OtherMap) {
			this->Add(Pair.Key, Pair.Value);
		}
	}

	ValueT& Emplace(KeyT&& Key, ValueT&& InVal) {
		auto Pair = this->insert_or_assign(Key, InVal);
		//auto Pair = this->try_emplace(Key, InVal);
		//if (!Pair.second)
		//	Pair.first->getSecond() = std::move(InVal);
		return Pair.first->getSecond();
	}

	ValueT& Emplace(const KeyT& Key) {
		return this->getOrInsertDefault(Key);
	}

	ValueT* Find(const KeyT& Key) {
		//return this->find(Key);
		auto Iter = this->find(Key);
		if (Iter != this->end()) {
			return &Iter->second;
		}
		return nullptr;
	}
	const ValueT* Find(const KeyT& Key) const {
		auto Iter = this->find(Key);
		if (Iter != this->end()) {
			return &Iter->second;
		}
		return nullptr;
	}

	bool IsEmpty() const { return this->IsEmpty(); }
	uint32 Num() const { return this->size(); }

	bool Remove(const KeyT& Val) { return this->erase(Val); }
	void Reserve(int32 Number) { this->reserve(Number); }
	void Reset() { this->clear(); }
	uint32 Num() { return this->size(); }

	int32 GetKeys(TArray<KeyT>& OutKeys) const {
		OutKeys.Reset();
		if (this->getNumEntries() == 0 && this->getNumTombstones() == 0) return 0;

		//TSet<KeyType> VisitedKeys;
		//VisitedKeys.Reserve(Num());
		OutKeys.Reserve(this->size());

		for (BucketT *P = this->getBuckets(), *E = this->getBucketsEnd(); P != E; ++P) {
			if (!KeyInfoT::isEqual(P->getFirst(), this->EmptyKey)) {
				if (!KeyInfoT::isEqual(P->getFirst(), this->TombstoneKey)) {
					//if (!VisitedKeys.Contains(It->Key)) {
						OutKeys.Add(P->getFirst());
					//	VisitedKeys.Add(P->getFirst());
					//}
				}
			}
		}
		return OutKeys.Num();
	}

};

} // namespace UE_CPP_Bridge

#endif
