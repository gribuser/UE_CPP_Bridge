#pragma once
#include "UE_CPP_Bridge_Setup.h"
#include <string>
#include <thread>
#include <list>
#if UE_CPP_BRIDGE_MUTEX_CLASSES_MODE == 1
#include <mutex>
#include <atomic>
#elif UE_CPP_BRIDGE_MUTEX_CLASSES_MODE == 2
#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#else
static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details");
#endif

namespace UE_CPP_Bridge {

#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
class FThreadsafeReadable;
void LockIn(const FThreadsafeReadable* Caller, bool Write);
void LockOut(const FThreadsafeReadable* Caller);
const int64 TrapLongLocksAt = 10000000 * 0.5;			// 0.5sec lock wait is kind of abnormal (to say the least)
const int64 TrapIgnoresLocksAfter = 10000000 * 2; // must be dubugger? We ignore this
#endif

class UE_CPP_BRIDGE_API FThreadsafeReadable {
private:
	mutable FThreadSafeCounter ReadersNum;
	mutable FCriticalSection WriteLock;
public:
	FThreadsafeReadable() {}
	mutable bool bMultyLockEnabled = false;
	mutable int LocksNum = 0;
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
	static const std::thread::id ZeroThread;
	mutable std::thread::id LockedBy;
	mutable std::thread::id UnlockedBy;
	mutable int64 LockedAt;
	int DebugLogN = 0;
	int RNum() { return ReadersNum.GetValue(); }
	FThreadsafeReadable(int ADebugLogN):DebugLogN(ADebugLogN) {}
#endif
	~FThreadsafeReadable() {
		UE_CPP_BRIDGE_DEV_TRAP(FreeState());
		WaitForFreeState();
	}
	void AcquireLock() const {
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//WriteLock.Lock();
		int64 TryLockAt = FDateTime::UtcNow().GetTicks();
		WriteLock.Lock();
		if (LocksNum == 0) {
			LockedAt = FDateTime::UtcNow().GetTicks();
			int64 LockingTook = LockedAt - TryLockAt;
			UE_CPP_BRIDGE_DEV_TRAP(LockingTook < TrapLongLocksAt || LockingTook > TrapIgnoresLocksAfter);
		}
#else
		WriteLock.Lock();
#endif
		if (bMultyLockEnabled) { LocksNum++; }
	}
	void ReleaseLock() const {
		if (bMultyLockEnabled) { LocksNum--; }
		if (LocksNum == 0) {
			int64 LockedFor = FDateTime::UtcNow().GetTicks() - LockedAt;
			WriteLock.Unlock();

			UE_CPP_BRIDGE_DEV_TRAP(LockedFor < TrapLongLocksAt || LockedFor > TrapIgnoresLocksAfter);
		}
	}
	// review me: do we need this WaitForFreeState at all, what's this???
	void WaitForFreeState() {
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		int i = 0;
#endif
		while (!FreeState()) {
			FPlatformProcess::Sleep(0.000001);
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
			i++;
			UE_CPP_BRIDGE_DEV_TRAP(i % 100000 == 0);
#endif
		}
	}
	bool FreeState() {
		return ReadersNum.GetValue() == 0
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
			&& LockedBy == ZeroThread
#endif
			;
	}
	void BeginRead() const {
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i>  BeginRead"),DebugLogN));
		LockIn(this, false);
#endif
		AcquireLock();
		ReadersNum.Increment();
		ReleaseLock();
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i>> BeginRead"),DebugLogN));
#endif
	}
	void EndRead() const {
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i<  EndRead"),DebugLogN));
#endif
		ReadersNum.Decrement();
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i<< EndRead"),DebugLogN));
		LockOut(this);
#endif
	}
	bool IsLocked() {
		return LockedBy == std::this_thread::get_id();
	}

	void BeginWrite() const {
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i>  BeginWrite"),DebugLogN));
		int64 i = 0;
		LockIn(this, true);
#endif
		AcquireLock();

#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		check(LockedBy == ZeroThread);
		LockedBy = std::this_thread::get_id();
#endif
		while (ReadersNum.GetValue() > 0) {
			FPlatformProcess::Sleep(0.000001);
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
			i++;
			UE_CPP_BRIDGE_DEV_TRAP(i % 10000000 != 0);
#endif
		}
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i>> BeginWrite"),DebugLogN));
#endif
	}

	void EndWrite() const {
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i<< EndWrite"),DebugLogN));
		LockOut(this);
		check(LockedBy == std::this_thread::get_id());
		UnlockedBy = LockedBy;
		LockedBy = ZeroThread;
#endif
		ReleaseLock();
	}
	//	bool TryBeginWrite() {
	//#if !UE_BUILD_SHIPPING
	//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i>  TryBeginWrite"),DebugLogN));
	//#endif
	//		bool TryResult = WriteLock.TryLock();
	//		if (!TryResult) {
	//			while (ReadersNum.GetValue() > 0) {}
	//		}
	//#if !UE_BUILD_SHIPPING
	//		if (DebugLogN) WriteP2PCosmosDebugLog(FString::Printf(TEXT("%i>> TryBeginWrite"),DebugLogN));
	//#endif
	//		return TryResult;
	//	}
};

class UE_CPP_BRIDGE_API FReadableScopeLockRead {
public:
	const FThreadsafeReadable* Section;
	FReadableScopeLockRead(const FThreadsafeReadable* ASection) :Section(ASection) {
		check(Section);
		Section->BeginRead();
	}
	~FReadableScopeLockRead() {
		Section->EndRead();
	}
};
class UE_CPP_BRIDGE_API FReadableScopeLockWrite{
public:
	FThreadsafeReadable* Section;
	FReadableScopeLockWrite(FThreadsafeReadable* ASection) :Section(ASection) {
		check(Section);
		Section->BeginWrite();
	}
	~FReadableScopeLockWrite() {
		Section->EndWrite();
	}
};
std::list<std::thread::id> UE_CPP_BRIDGE_API WhoIsLocking(const FThreadsafeReadable* Caller);
};