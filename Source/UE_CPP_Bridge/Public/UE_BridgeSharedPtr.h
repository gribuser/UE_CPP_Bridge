#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
#include <memory>
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "Templates/SharedPointer.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's TSharedPtr functionality with std::shared_ptr
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1

/**
 * ESPMode is used select between either 'fast' or 'thread safe' shared pointer types.
 * This is only used by templates at compile time to generate one code path or another.
 */
enum class ESPMode : uint8_t
{
	/** Forced to be not thread-safe. */
	NotThreadSafe = 0,

	/** Thread-safe, never spin locks, but slower */
	ThreadSafe = 1
};

template< class ObjectType, ESPMode InMode >
class TSharedPtr : private std::shared_ptr<ObjectType> {
	using std::shared_ptr<ObjectType>::shared_ptr;
	//FORCEINLINE TSharedPtr(ObjectType* ObjectPtr) : std::shared_ptr<ObjectType>(ObjectPtr);

	const bool IsValid() const { return !!this && use_count() > 0; }
	void Reset() { reset(); }
	ObjectType* Get() const { return get(); }
};

#endif
