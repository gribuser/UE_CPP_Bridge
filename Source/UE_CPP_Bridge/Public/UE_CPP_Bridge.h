#pragma once
// =========================== SETUP =====================

/// Defines the used implementation library
/// 0 - std::* implementation (sts::array, std::mutex, etc)
/// 1 - UE implementation (TArray, FCriticalSection, etc)
#ifndef UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE
#define UE_CPP_BRIDGE_DEFAULT_CLASSES_MODE 1
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
#ifndef UE_CPP_BRIDGE_DEV_TRAP(Val)
#define UE_CPP_BRIDGE_DEV_TRAP(Val) check(Val);
#endif

// =========================== IMPLEMENTATION =====================
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UE_BridgeLocking.h"
#include "UE_BridgeArray.h"
#include "UE_BridgeMap.h"
#include "UE_BridgeSet.h"
#include "UE_BridgeThreadedContainers.h"

class FUE_CPP_BridgeModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

