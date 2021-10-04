#pragma once
#include "UE_CPP_Bridge_Setup.h"

#if UE_CPP_BRIDGE_RUNNABLE_CLASSES_MODE == 2
	#include "HAL/Runnable.h"
	#include "HAL/RunnableThread.h"
#endif

namespace UE_CPP_Bridge {
class UE_CPP_BRIDGE_API FFInishibleRunnable: public FRunnable {
public:
	bool Finished = false;
	bool bStopThread = false;
	bool Paused = true;
	virtual void Stop() override { bStopThread = true; }
};

};