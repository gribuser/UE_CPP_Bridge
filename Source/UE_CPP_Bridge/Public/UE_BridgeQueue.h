#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
#include <atomic>
#include <utility>
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "Containers/Queue.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's TQueue functionality with std::*
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1

namespace UE_CPP_Bridge {

/**
 * Enumerates concurrent queue modes.
 */
enum class EQueueMode {
	/** Multiple-producers, single-consumer queue. */
	Mpsc,

	/** Single-producer, single-consumer queue. */
	Spsc
};

/**
 * Template for queues.
 *
 * This template implements an unbounded non-intrusive queue using a lock-free linked
 * list that stores copies of the queued items. The template can operate in two modes:
 * Multiple-producers single-consumer (MPSC) and Single-producer single-consumer (SPSC).
 *
 * The queue is thread-safe in both modes. The Dequeue() method ensures thread-safety by
 * writing it in a way that does not depend on possible instruction reordering on the CPU.
 * The Enqueue() method uses an atomic compare-and-swap.
 *
 * @param T The type of items stored in the queue.
 */
template <typename T, EQueueMode DontCare = EQueueMode::Mpsc>
class TQueue {
public:
	using FElementType = T;

	/** Default constructor. */
	TQueue() { Head = Tail = new TNode(); }

	/** Destructor. */
	~TQueue() {
		while (Tail != nullptr) {
			TNode* Node = Tail;
			Tail = Tail->NextNode;

			delete Node;
		}
	}

	/**
	 * Removes and returns the item from the tail of the queue.
	 *
	 * @param OutValue Will hold the returned value.
	 * @return true if a value was returned, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 * @see Empty, Enqueue, IsEmpty, Peek, Pop
	 */
	bool Dequeue(FElementType& OutItem) {
		TNode* Popped = Tail->NextNode;

		if (Popped == nullptr) { return false; }

		OutItem = std::move(Popped->Item);
		TNode* OldTail = Tail;
		Tail = Popped;
		Tail->Item = FElementType();
		delete OldTail;

		return true;
	}

	/**
	 * Empty the queue, discarding all items.
	 *
	 * @note To be called only from consumer thread.
	 * @see Dequeue, IsEmpty, Peek, Pop
	 */
	void Empty() {
		while (Pop())
			;
	}

	/**
	 * Adds an item to the head of the queue.
	 *
	 * @param Item The item to add.
	 * @return true if the item was added, false otherwise.
	 * @note To be called only from producer thread(s).
	 * @see Dequeue, Pop
	 */
	bool Enqueue(const FElementType& Item) {
		TNode* NewNode = new TNode(Item);

		if (NewNode == nullptr) { return false; }

		// https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
		// put the current value of Head into OldHead
		TNode* OldHead = Head.load(std::memory_order_relaxed);

		// now make NewNode the new Head, but if the Head
		// is no longer what's stored in OldHead
		// (some other thread must have inserted a node just now)
		// then put that new Head into OldHead and try again
		while (!Head.compare_exchange_weak(OldHead, NewNode, std::memory_order_release,
																			 std::memory_order_relaxed))
			; // the body of the loop is empty

		// put the NewNode into OldHead->NextNode
		OldHead->NextNode = NewNode;

		return true;
	}

	/**
	 * Adds an item to the head of the queue.
	 *
	 * @param Item The item to add.
	 * @return true if the item was added, false otherwise.
	 * @note To be called only from producer thread(s).
	 * @see Dequeue, Pop
	 */
	bool Enqueue(FElementType&& Item) {
		TNode* NewNode = new TNode(std::move(Item));

		if (NewNode == nullptr) { return false; }

		TNode* OldHead = Head.load(std::memory_order_relaxed);
		while (!Head.compare_exchange_weak(OldHead, NewNode, std::memory_order_release,
																			 std::memory_order_relaxed))
			; // the body of the loop is empty
		OldHead->NextNode = NewNode;
		return true;
	}

	/**
	 * Checks whether the queue is empty.
	 *
	 * @return true if the queue is empty, false otherwise.
	 * @note To be called only from consumer thread.
	 * @see Dequeue, Empty, Peek, Pop
	 */
	bool IsEmpty() const { return (Tail->NextNode == nullptr); }

	/**
	 * Peeks at the queue's tail item without removing it.
	 *
	 * @param OutItem Will hold the peeked at item.
	 * @return true if an item was returned, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 * @see Dequeue, Empty, IsEmpty, Pop
	 */
	bool Peek(FElementType& OutItem) const {
		if (Tail->NextNode == nullptr) { return false; }

		OutItem = Tail->NextNode->Item;

		return true;
	}

	/**
	 * Peek at the queue's tail item without removing it.
	 *
	 * This version of Peek allows peeking at a queue of items that do not allow
	 * copying, such as TUniquePtr.
	 *
	 * @return Pointer to the item, or nullptr if queue is empty
	 */
	FElementType* Peek() {
		if (Tail->NextNode == nullptr) { return nullptr; }

		return &Tail->NextNode->Item;
	}

	FORCEINLINE const FElementType* Peek() const { return const_cast<TQueue*>(this)->Peek(); }

	/**
	 * Removes the item from the tail of the queue.
	 *
	 * @return true if a value was removed, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 * @see Dequeue, Empty, Enqueue, IsEmpty, Peek
	 */
	bool Pop() {
		TNode* Popped = Tail->NextNode;

		if (Popped == nullptr) { return false; }

		TNode* OldTail = Tail;
		Tail = Popped;
		Tail->Item = FElementType();
		delete OldTail;

		return true;
	}

private:
	/** Structure for the internal linked list. */
	struct TNode {
		/** Holds a pointer to the next node in the list. */
		TNode* volatile NextNode;

		/** Holds the node's item. */
		FElementType Item;

		/** Default constructor. */
		TNode() : NextNode(nullptr) {}

		/** Creates and initializes a new node. */
		explicit TNode(const FElementType& InItem) : NextNode(nullptr), Item(InItem) {}

		/** Creates and initializes a new node. */
		explicit TNode(FElementType&& InItem) : NextNode(nullptr), Item(std::move(InItem)) {}
	};

	///** Holds a pointer to the head of the list. */
	std::atomic<TNode*> Head;
	// MS_ALIGN(16) TNode* volatile Head GCC_ALIGN(16);

	/** Holds a pointer to the tail of the list. */
	TNode* Tail;

private:
	/** Hidden copy constructor. */
	TQueue(const TQueue&) = delete;

	/** Hidden assignment operator. */
	TQueue& operator=(const TQueue&) = delete;
};

} // namespace UE_CPP_Bridge

#endif
