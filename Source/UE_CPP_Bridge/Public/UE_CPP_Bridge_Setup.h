#pragma once
/// Defines the used implementation library
/// 1 - std::* implementation (sts::array, std::mutex, etc)
/// 2 - UE implementation (TArray, FCriticalSection, etc)
#ifndef UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE
#define UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE 2
#endif

#ifndef UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE
#define UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE
#endif

#ifndef UE_CPP_BRIDGE_MUTEX_CLASSES_MODE
#define UE_CPP_BRIDGE_MUTEX_CLASSES_MODE UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE
#endif

/// Should we check for the possible deadlocks for our mutex? Remember to turn it off in production
#ifndef WITH_THREAD_INTERLOCKING_DIAGNOSTICS
#define WITH_THREAD_INTERLOCKING_DIAGNOSTICS 1
#endif

/// Deadlock detection and other dev-diagnostics call this template to fire this
/// kinda "exception" when something goes wrong. Change to your liking.
#ifndef UE_CPP_BRIDGE_DEV_TRAP
#define UE_CPP_BRIDGE_DEV_TRAP(Val) check(Val);
#endif