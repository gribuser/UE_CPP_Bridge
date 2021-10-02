#pragma once
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 0
#include <vector>
#include <random>
#include <algorithm>
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
#include "CoreMinimal.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

namespace UE_CPP_Bridge {

template<typename InElementType>

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 0
class TBridgeArray: public std::vector<InElementType> {
public:
	using std::vector<InElementType>::vector;
	size_t Num() const { return size(); }
	void Reset() { clear(); }
	size_t Add(InElementType&& Item) { push_back(Item); return size() - 1; }
	size_t Add(const InElementType& Item) { push_back(Item); return size() - 1; }
	bool Contains(const InElementType& Item) const {
		for (const InElementType& R : *this)
			if (R == Item)
				return true;
		return false;
	}
	void Reserve(size_t N) { reserve(N); }
	void SetNum(size_t N) { resize(N); }

	// Hacked here, could break
	InElementType* GetData() const { return (InElementType*)data(); }
	void RemoveAt(size_t N) { erase(begin() + N); }
	void RemoveAt(size_t N, size_t Count, bool fake) { erase(begin() + N, begin() + N + Count); }
	size_t GetTypeSize() const { return sizeof(InElementType); }
	void Empty() { clear(); }
	void AddUnique(const InElementType& Item) { if (!Contains(Item)) Add(Item); }
	void AddUnique(const InElementType&& Item) { if (!Contains(Item)) Add(Item); }
	template <typename Predicate>
	const InElementType* FindByPredicate(Predicate Pred) const {
		for (const InElementType& R : *this)
			if (Pred(R))
				return &R;
		return NULL;
	}
	InElementType& Top() { return back(); }
	const InElementType& Top() const { return back(); }

	template <typename Predicate>
	size_t RemoveAll(const Predicate Pred) {
		size_t Out = 0;
		for (int i = Num() - 1; i >= 0; i--) 		{
			if (Pred((*this)[i])) {
				erase(begin() + i);
				Out++;
			}
		}
		return Out;
	}
	template <typename Predicate>
	int IndexOfByPredicate(const Predicate Pred) const {
		for (size_t i = 0; i < size(); i++) {
			if (Pred((*this)[i]))
				return i;
		}
		return INDEX_NONE;
	}
	size_t Find(InElementType F) const {
		for (size_t i = 0; i < size(); i++) {
			if (F == (*this)[i])
				return i;
		}
		return INDEX_NONE;
	}

	size_t Remove(const InElementType& Item) {
		return RemoveAll([&Item](InElementType& Element) { return Element == Item; });
	}
	// optimize me?
	void Append(const InElementType* Ptr, size_t Count) {
		check(Ptr != nullptr || Count == 0);
		for (size_t i = 0; i < Count; i++) {
			push_back(Ptr[i]);
		}
	}
	template <typename... ArgsType>
	size_t Emplace(ArgsType&&... Args)
	{
		push_back(InElementType(Args...));
		return size() - 1;
	}

	void Sort(){std::sort(begin(), end(), std::greater<InElementType>());}
	void Shuffle() {
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(begin(), end(), g);
	}
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
class TBridgeArray: public TArray<InElementType> {
public:
	using TArray<InElementType>::TArray;
	void Shuffle() {
		for (int i = Num() - 1; i > 0; i--) {
			int j = FMath::RandRange(0, Num()-1);
			InElementType temp = (*this)[i];
			(*this)[i] = (*this)[j];
			(*this)[j] = temp;
		}
	}
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif
};

};