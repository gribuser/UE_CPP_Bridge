#pragma once
#include "UE_BridgeLocking.h"
#include <thread>
#include <sstream>
#include <map>
#include "UE_BridgeArray.h"

namespace UE_CPP_Bridge {
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS

std::string StreamString() {
	std::stringstream ss;
	ss << std::this_thread::get_id();
	return ss.str();
}

// Could not make this work with UE TMap, so std::map will do well anyway
std::map<std::thread::id, UE_CPP_Bridge::TBridgeArray<const FThreadsafeReadable*>> LocksLog;
std::map<std::thread::id, UE_CPP_Bridge::TBridgeArray<bool>> RWLog;
FCriticalSection LockingDiagnosticsMapLock;

void LockIn(const FThreadsafeReadable* Caller, bool Write) {
	FScopeLock ScopeLock(&LockingDiagnosticsMapLock);
	std::thread::id MyThreadID = std::this_thread::get_id();
	auto SearchResult = LocksLog.find(MyThreadID);
	UE_CPP_Bridge::TBridgeArray<const FThreadsafeReadable*>* ThreadRec;
	UE_CPP_Bridge::TBridgeArray<bool>* RWRec;
	if (SearchResult != LocksLog.end()) {
		ThreadRec = &SearchResult->second;
		RWRec = &RWLog[MyThreadID];
	} else {
		auto InsertResult1 = LocksLog.emplace( MyThreadID, UE_CPP_Bridge::TBridgeArray<FThreadsafeReadable*>() );
		ThreadRec = &InsertResult1.first->second;
		auto InsertResult2 = RWLog.emplace( MyThreadID, UE_CPP_Bridge::TBridgeArray<bool>() );
		RWRec = &InsertResult2.first->second;
	}
	int ExistingLockN = ThreadRec->Find(Caller);
	// This thread had locked this mutex already
	UE_CPP_BRIDGE_DEV_TRAP(ExistingLockN == INDEX_NONE || !(*RWRec)[ExistingLockN] && !Write);

	for (size_t i = 0; i < ThreadRec->Num(); i++) {
		const FThreadsafeReadable* PrevLocker = (*ThreadRec)[i];
		bool PrevWrite = (*RWRec)[i];
		for (auto& AnotherLocker : LocksLog) {
			int ANotherLockedMyPrevN = AnotherLocker.second.Find(PrevLocker);
			if (ANotherLockedMyPrevN == INDEX_NONE) continue;
			// Readlocks do not block each other!
			if (!PrevWrite && !RWLog[AnotherLocker.first][ANotherLockedMyPrevN]) continue;

			int ANotherLockedMyCurrentN = AnotherLocker.second.Find(Caller);
			if (ANotherLockedMyCurrentN == INDEX_NONE) continue;
			if (!Write && !RWLog[AnotherLocker.first][ANotherLockedMyCurrentN]) continue;
			// Cross-thread deadlock: thread A locks [1] then [2], thread B locks [2] then [1]
			UE_CPP_BRIDGE_DEV_TRAP(ANotherLockedMyCurrentN > ANotherLockedMyPrevN);
			//UE_CPP_BRIDGE_DEV_TRAP(!(
			//	AnotherLocker.second.Contains(Caller)
			//	&& AnotherLocker.second.Contains(PrevLocker)
			//	));
		}
	}
	ThreadRec->Add(Caller);
	RWRec->Add(Write);
}

void LockOut(const FThreadsafeReadable* Caller) {
	FScopeLock ScopeLock(&LockingDiagnosticsMapLock);
	std::thread::id MyThreadID = std::this_thread::get_id();
	auto SearchResult = LocksLog.find(MyThreadID);
	if (SearchResult != LocksLog.end()) {
		UE_CPP_Bridge::TBridgeArray<const FThreadsafeReadable*>* ThreadRec = &SearchResult->second;
		int ThisLockN = ThreadRec->Find(Caller);
		UE_CPP_BRIDGE_DEV_TRAP(ThisLockN != INDEX_NONE);

		ThreadRec->RemoveAt(ThisLockN);
		RWLog[MyThreadID].RemoveAt(ThisLockN);
	} else { UE_CPP_BRIDGE_DEV_TRAP(0); }
}

#endif
};


