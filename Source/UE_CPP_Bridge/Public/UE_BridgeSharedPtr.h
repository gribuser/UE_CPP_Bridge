#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1
#include <memory>
#include <type_traits>
#elif UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 2
#include "Templates/SharedPointer.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details")
#endif

// We emulate base UE's TSharedPtr functionality with std::shared_ptr
#if UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE == 1

namespace UE_CPP_Bridge {

/**
 * ESPMode is used select between either 'fast' or 'thread safe' shared pointer types.
 * This is only used by templates at compile time to generate one code path or another.
 */
// PJ: These modes do nothing. They are here only for compatibility
enum class ESPMode : uint8_t {
	/** Forced to be not thread-safe. */
	NotThreadSafe = 0,

	/** Thread-safe, never spin locks, but slower */
	ThreadSafe = 1
};

template< class ObjectType, ESPMode InMode = ESPMode::NotThreadSafe > // InMode do nothing
class TSharedPtr : public std::shared_ptr<ObjectType> {
	typedef typename std::shared_ptr<ObjectType> Super;
	//using std::shared_ptr<ObjectType>::use_count;
	//using std::shared_ptr<ObjectType>::reset;
	//using std::shared_ptr<ObjectType>::get;
	//FORCEINLINE TSharedPtr(ObjectType* ObjectPtr) : std::shared_ptr<ObjectType>(ObjectPtr);
public:
	using Super::shared_ptr; // Inheriting constructors
	//explicit TSharedPtr(const ObjectType* ObjectPtr): Super(ObjectPtr) {}
	const bool IsValid() const { return !!this && Super::use_count() > 0; }
	void Reset() { Super::reset(); }
	ObjectType* Get() const { return Super::get(); }
	ObjectType* Get() { return Super::get(); }
};

template< class ObjectType, ESPMode InMode = ESPMode::NotThreadSafe > // InMode do nothing
class TSharedRef: public TSharedPtr<ObjectType, InMode> {
public:
	explicit TSharedRef(ObjectType* ObjectPtr): TSharedPtr<ObjectType, InMode>(ObjectPtr) {
		static_assert(!std::is_null_pointer_v<ObjectType*>, "TSharedRef cannot take null-object");
	}
};

template< class ObjectType, ESPMode InMode = ESPMode::NotThreadSafe >
class TSharedFromThis: public std::enable_shared_from_this<ObjectType> {
	using std::enable_shared_from_this<ObjectType>::shared_from_this;

public:
	const bool IsValid() const { return !!this && shared_from_this().use_count() > 0; }
	void Reset() { shared_from_this().reset(); }
	ObjectType* Get() const { return shared_from_this().get(); }
};

template< class ObjectType, ESPMode InMode >
class TWeakPtr: public std::weak_ptr<ObjectType> {
	using std::weak_ptr<ObjectType>::lock;
	using std::weak_ptr<ObjectType>::use_count;
	using std::weak_ptr<ObjectType>::reset;
	//FORCEINLINE TSharedPtr(ObjectType* ObjectPtr) : std::shared_ptr<ObjectType>(ObjectPtr);
public:
	const bool IsValid() const { return !!this && use_count() > 0; }
	void Reset() { reset(); }
	TSharedPtr<ObjectType, InMode> Pin() { return lock(); }
	//ObjectType* Get() const { return get(); }
};

} // namespace UE_CPP_Bridge

#endif
