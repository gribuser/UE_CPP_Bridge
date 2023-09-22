#pragma once

#ifndef UE_CPP_BRIDGE_API
#define UE_CPP_BRIDGE_API DLLIMPORT
#endif

/// Defines the used implementation library
/// 1 - std::* implementation (sts::array, std::mutex, etc)
/// 2 - UE implementation (TArray, FCriticalSection, etc)
#ifndef UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE
	// We are inside UE, so we (by default) use UE's classes
	#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING == 1
		#define UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE 2
	#else
		#define UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE 1
	#endif
#endif

#ifndef UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE
	#define UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE
#endif

#ifndef UE_CPP_BRIDGE_MUTEX_CLASSES_MODE
	#define UE_CPP_BRIDGE_MUTEX_CLASSES_MODE UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE
#endif

#ifndef UE_CPP_BRIDGE_RUNNABLE_CLASSES_MODE
	#define UE_CPP_BRIDGE_RUNNABLE_CLASSES_MODE UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE
#endif

/// Should we check for the possible deadlocks for our mutex? Remember to turn it off in production
#ifndef WITH_THREAD_INTERLOCKING_DIAGNOSTICS
	#if UE_BUILD_SHIPPING
		#define WITH_THREAD_INTERLOCKING_DIAGNOSTICS 0
		#define WITH_LONG_LOCKING_TRAPS 0
#else
		#define WITH_THREAD_INTERLOCKING_DIAGNOSTICS 1
		#define WITH_LONG_LOCKING_TRAPS 0
	#endif
#endif

/// Deadlock detection and other dev-diagnostics call this template to fire this
/// kinda "exception" when something goes wrong. Change to your liking.
#ifndef UE_CPP_BRIDGE_DEV_TRAP
	#define UE_CPP_BRIDGE_DEV_TRAP(Val) check(Val);
#endif

// Used to switch on|off autotests in UE
//#define WITH_DEV_AUTOMATION_TESTS_X 1
#define WITH_DEV_AUTOMATION_TESTS_X WITH_DEV_AUTOMATION_TESTS
