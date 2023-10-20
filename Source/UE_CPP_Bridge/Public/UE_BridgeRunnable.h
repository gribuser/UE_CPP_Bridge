#pragma once
#include "UE_CPP_Bridge_Setup.h"
#include "UE_BridgeGenericPlatform.h"
#include <thread>
#include <string>

namespace UE_CPP_Bridge {
class UE_CPP_BRIDGE_API ThreadWorker {
public:
	void SetName(std::wstring AName);
	std::string ThreadName;
	bool Finished = false;
	bool bStopThread = false;
	bool Paused = true;
	void Start();
	void Stop() { check(Thread); bStopThread = true; }
	void Terminate();
	virtual ~ThreadWorker();
protected:
	virtual uint32 Run() = 0;
private:
	std::thread* Thread = NULL;
};
};