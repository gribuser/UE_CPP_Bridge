#pragma once
#include "Modules/ModuleManager.h"
#include "UE_CPP_BridgeClasses.h"
class FUE_CPP_BridgeModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

