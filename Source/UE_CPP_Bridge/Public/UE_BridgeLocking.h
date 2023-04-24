#pragma once
#include "UE_CPP_Bridge_Setup.h"
#include <string>
#include <list>
#include <thread>
#include <mutex>
#include <atomic>
//#elif UE_CPP_BRIDGE_MUTEX_CLASSES_MODE == 2
//#include "CoreMinimal.h"
//#include "HAL/CriticalSection.h"
//#include "HAL/ThreadSafeCounter.h"
//#else
//static_assert(0, "Unknown implementation ID, see UE_CPP_BRIDGE_CONTAINER_CLASSES_MODE description for details");
//#endif

#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1 || WITH_LONG_LOCKING_TRAPS == 1
	#define WITH_ADDITIONAL_LOCKING_VARS 1
#else
	#define WITH_ADDITIONAL_LOCKING_VARS 0
#endif

namespace UE_CPP_Bridge {

#if WITH_LONG_LOCKING_TRAPS == 1
//const int64 TrapLongLocksAt = 10000000 * 0.3;
const int64 TrapLongLocksAt = 10000000 * 2;
const int64 TrapShortLocksAt = 10000000 * 0.05;
const int64 TrapIgnoresLocksAfter = 10000000 * 2;		// must be dubugger? We ignore this
#endif

class UE_CPP_BRIDGE_API FThreadsafeReadable {
private:
	mutable std::atomic<std::uint32_t> ReadersNum;
	mutable std::recursive_mutex WriteLock;
public:
	FThreadsafeReadable() {}
#if WITH_ADDITIONAL_LOCKING_VARS == 1
	mutable bool bMultiLockEnabled = false;
	int DebugLogN = 0;
	mutable int LocksNum = 0;
	static const std::thread::id ZeroThread;
	mutable std::thread::id LastReader;
	mutable std::thread::id LockedBy;
	mutable std::thread::id UnlockedBy;
	FThreadsafeReadable(int ADebugLogN):DebugLogN(ADebugLogN) {}
#endif


#if WITH_LONG_LOCKING_TRAPS == 1
	mutable int64 LockedAt;
#endif

	~FThreadsafeReadable() {
		UE_CPP_BRIDGE_DEV_TRAP(FreeState());
		WaitForFreeState();
	}
	void AcquireLock() const {
#if WITH_LONG_LOCKING_TRAPS == 1
		int64 TryLockAt = FDateTime::UtcNow().GetTicks();
#endif
		WriteLock.lock();
		//WriteLock.lock();
#if WITH_LONG_LOCKING_TRAPS == 1
		if (LocksNum == 0) {
			LockedAt = FDateTime::UtcNow().GetTicks();
			int64 LockingTook = LockedAt - TryLockAt;
			UE_CPP_BRIDGE_DEV_TRAP(LockingTook < TrapLongLocksAt || LockingTook >= TrapIgnoresLocksAfter);
			UE_CPP_BRIDGE_DEV_TRAP(bMultiLockEnabled || LockingTook < TrapShortLocksAt || LockingTook > TrapIgnoresLocksAfter);
		}
#endif
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
		check(LockedBy == ZeroThread || bMultiLockEnabled);
		LockedBy = std::this_thread::get_id();
#endif
#if WITH_ADDITIONAL_LOCKING_VARS == 1
		if (bMultiLockEnabled) { LocksNum++; }
#endif
	}
	void ReleaseLock() const {
#if WITH_ADDITIONAL_LOCKING_VARS == 1
		if (bMultiLockEnabled) { LocksNum--; }
		if (LocksNum == 0) {
	#if WITH_LONG_LOCKING_TRAPS == 1
			int64 LockedFor = FDateTime::UtcNow().GetTicks() - LockedAt;
			UE_CPP_BRIDGE_DEV_TRAP(LockedFor < TrapLongLocksAt || LockedFor >= TrapIgnoresLocksAfter);
	#endif
	#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
			UnlockedBy = LockedBy;
			LockedBy = ZeroThread;
	#endif
		}
#endif
		WriteLock.unlock();
	}
	// review me: do we need this WaitForFreeState at all, what's this???
	void WaitForFreeState() {
#if WITH_LONG_LOCKING_TRAPS == 1
		int i = 0;
#endif
		while (!FreeState()) {
			FPlatformProcess::Sleep(0.000001);
#if WITH_LONG_LOCKING_TRAPS == 1
			i++;
			UE_CPP_BRIDGE_DEV_TRAP(i % 100000 == 0);
#endif
		}
	}
	bool FreeState() {
		return ReadersNum == 0
#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
			&& LockedBy == ZeroThread
#endif
			;
	}
	void BeginRead() const;
	void EndRead() const;

#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS
	bool IsLocked(bool OkVal = true) {
		return LockedBy == std::this_thread::get_id();
	}
	bool IsLockedRead(bool OkVal = true) {
		// not 100% guarantee ReadersNum means THIS thread had a read-lock, but should do for now
		return LockedBy == std::this_thread::get_id() || ReadersNum > 0;
	}
#else
	bool IsLocked(bool OkVal = true) {return OkVal;}
	bool IsLockedRead(bool OkVal = true) {return OkVal;}
#endif

	void BeginWrite() const;

	void EndWrite() const;

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

#if WITH_THREAD_INTERLOCKING_DIAGNOSTICS == 1
void LockIn(const FThreadsafeReadable* Caller, bool Write);
void LockOut(const FThreadsafeReadable* Caller);
#endif

class UE_CPP_BRIDGE_API FReadableScopeLockRead {
public:
	const FThreadsafeReadable* Section;
	FReadableScopeLockRead(const FThreadsafeReadable* ASection) :Section(ASection) {
		check(Section);
		Section->BeginRead();
	}
	~FReadableScopeLockRead() {
		if (Section != NULL) { Section->EndRead(); }
	}
	void Release() { Section->EndRead(); Section = NULL; }
};
class UE_CPP_BRIDGE_API FReadableScopeLockWrite{
public:
	const FThreadsafeReadable* Section;
	FReadableScopeLockWrite(const FThreadsafeReadable* ASection) :Section(ASection) {
		check(Section);
		Section->BeginWrite();
	}
	~FReadableScopeLockWrite() {
		if (Section != NULL) { Section->EndWrite(); }
	}
	void Release() { Section->EndWrite(); Section = NULL; }
};
std::list<std::thread::id> UE_CPP_BRIDGE_API WhoIsLocking(const FThreadsafeReadable* Caller);
};