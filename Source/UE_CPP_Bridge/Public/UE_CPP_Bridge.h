#pragma once
#include "UE_CPP_Bridge_Setup.h"
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

