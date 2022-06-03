#pragma once
#include "UE_BridgeLocking.h"
#include "UE_BridgeArray.h"
#include <sstream>
#include <map>

namespace UE_CPP_Bridge {
#if WITH_ADDITIONAL_LOCKING_VARS
const std::thread::id FThreadsafeReadable::ZeroThread;
#endif

void FThreadsafeReadable::BeginRead() const {
	{
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i>  BeginRead"),DebugLogN));
		LockIn(this, false);
#endif
		AcquireLock();
		ReadersNum++;
		LastReader = std::this_thread::get_id();
		ReleaseLock();
	}
}

void FThreadsafeReadable::EndRead() const {
	ReadersNum--;
	#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
	LockOut(this);
	#endif
}

void FThreadsafeReadable::BeginWrite() const {
	#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
	LockIn(this, true);
	#endif
	AcquireLock();

	#if WITH_LONG_LOCKING_TRAPS == 1
	int64 WaitStartedAt = FDateTime::UtcNow().GetTicks();
	#endif
	while (ReadersNum > 0) {
		FPlatformProcess::Sleep(0.00001);
		#if WITH_LONG_LOCKING_TRAPS == 1
		int64 Now = FDateTime::UtcNow().GetTicks();
		UE_CPP_BRIDGE_DEV_TRAP(Now - WaitStartedAt < TrapLongLocksAt || Now - WaitStartedAt >= TrapIgnoresLocksAfter);
		#endif
	}
}
void FThreadsafeReadable::EndWrite() const {
	#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
	LockOut(this);
	check(LockedBy == std::this_thread::get_id());
	#endif
	ReleaseLock();
}


#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1

std::string StreamString() {
	std::stringstream ss;
	ss << std::this_thread::get_id();
	return ss.str();
}

// Could not make thread::id work with UE TMap, so std::map will do well anyway
std::map<std::thread::id, TArray<const FThreadsafeReadable*>> LocksLog;
std::map<std::thread::id, TArray<bool>> RWLog;
FCriticalSection LockingDiagnosticsMapLock;

void LockIn(const FThreadsafeReadable* Caller, bool Write) {
	FScopeLock ScopeLock(&LockingDiagnosticsMapLock);
	std::thread::id MyThreadID = std::this_thread::get_id();
	auto SearchResult = LocksLog.find(MyThreadID);
	TArray<const FThreadsafeReadable*>* ThreadRec;
	TArray<bool>* RWRec;
	if (SearchResult != LocksLog.end()) {
		ThreadRec = &SearchResult->second;
		RWRec = &RWLog[MyThreadID];
	} else {
		auto InsertResult1 = LocksLog.emplace( MyThreadID, TArray<FThreadsafeReadable*>() );
		ThreadRec = &InsertResult1.first->second;
		auto InsertResult2 = RWLog.emplace( MyThreadID, TArray<bool>() );
		RWRec = &InsertResult2.first->second;
	}
	int ExistingLockN = ThreadRec->Find(Caller);
	// This thread had locked this mutex already
	UE_CPP_BRIDGE_DEV_TRAP(Caller->bMultyLockEnabled || ExistingLockN == INDEX_NONE || !(*RWRec)[ExistingLockN] && !Write);

	// If this is a uniq lock it's ok. And if it's all about read-locking, you can stack it
	// as much as you like. Otherwise we need some checks here and we are not going to
	// log anything (as we have the lock already and new lock will not be set)
	if (ExistingLockN != INDEX_NONE && (Write || (*RWRec)[ExistingLockN])) {

		// You can't upgrade read-only lock to read-write lock!
		UE_CPP_BRIDGE_DEV_TRAP((*RWRec)[ExistingLockN]);

		// You can only re-lock if bMultyLockEnabled was set, so generally it's suspicious
		UE_CPP_BRIDGE_DEV_TRAP(Caller->bMultyLockEnabled);

		return;
	}
	//if (ExistingLockN != INDEX_NONE && !Write) {
	//	// We have this log already anyway, so no chance to deadlock more than we did already
	//	return;
	//}

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
			UE_CPP_BRIDGE_DEV_TRAP(ANotherLockedMyCurrentN >= ANotherLockedMyPrevN);
			//UE_CPP_BRIDGE_DEV_TRAP(!(
			//	AnotherLocker.second.Contains(Caller)
			//	&& AnotherLocker.second.Contains(PrevLocker)
			//	));
		}
	}
	ThreadRec->Add(Caller);
	RWRec->Add(Write);
	// Just to force compiler to link WhoIsLocking for immediate call
	if (0) WhoIsLocking(NULL);
}

std::list<std::thread::id> WhoIsLocking(const FThreadsafeReadable* Caller) {
	std::list<std::thread::id> Out;
	for (auto& Thread : LocksLog) {
		for (const FThreadsafeReadable* LockPointer : Thread.second) {
			if (Caller == LockPointer) {
				Out.push_back(Thread.first);
//				goto NEXTTHREAD;
			}
		}
//		NEXTTHREAD:;
	}
	return Out;
}

void LockOut(const FThreadsafeReadable* Caller) {
	if (Caller->LocksNum > 1) { return; }
	FScopeLock ScopeLock(&LockingDiagnosticsMapLock);
	std::thread::id MyThreadID = std::this_thread::get_id();
	auto SearchResult = LocksLog.find(MyThreadID);
	if (SearchResult != LocksLog.end()) {
		TArray<const FThreadsafeReadable*>* ThreadRec = &SearchResult->second;
		int ThisLockN = ThreadRec->Find(Caller);
		if (ThisLockN != INDEX_NONE) {
			ThreadRec->RemoveAt(ThisLockN);
			RWLog[MyThreadID].RemoveAt(ThisLockN);
		} else {
			// This should not happen, but it's too hard to fix, so I'll just ignore it as it's
			// not a production code anyways
			// UE_CPP_BRIDGE_DEV_TRAP();
		}

	} else { UE_CPP_BRIDGE_DEV_TRAP(0); }
}

#endif
};


