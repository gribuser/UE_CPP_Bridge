#pragma once

#ifndef P2P_BUILD_FOR_STANDALONE_SERVER
#include "Modules/ModuleManager.h"
#include "UE_CPP_BridgeClasses.h"
class FUE_CPP_BridgeModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};
#endif
