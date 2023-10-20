#pragma once
#include "UE_BridgeRunnable.h"
#include <locale> 
#include <codecvt>

#ifdef _WIN32
	#if P2P_BUILD_FOR_STANDALONE_SERVER
		#include <windows.h>
	#else
		#include "Windows/MinWindows.h"
	#endif
#elif defined(__linux__)
#include <sys/prctl.h>
#endif


namespace UE_CPP_Bridge {

#ifdef _WIN32
const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO {
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName(uint32_t dwThreadID, const char* threadName) {
	// DWORD dwThreadID = ::GetThreadId( static_cast<HANDLE>( t.native_handle() ) );
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
}
void SetThreadName(const char* threadName) {
	SetThreadName(GetCurrentThreadId(), threadName);
}

void SetThreadName(std::thread* thread, const char* threadName) {
	DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread->native_handle()));
	SetThreadName(threadId, threadName);
}

#elif defined(__linux__)
#include <sys/prctl.h>
void SetThreadName(const char* threadName) {
	prctl(PR_SET_NAME, threadName, 0, 0, 0);
}

#else
void SetThreadName(std::thread* thread, const char* threadName) {
	auto handle = thread->native_handle();
	pthread_setname_np(handle, threadName);
}
#endif

ThreadWorker::~ThreadWorker() {
	if (Thread != NULL) {
		Terminate();
	}
}
#pragma warning (push, 3)
#pragma warning (disable : 4996)
void ThreadWorker::SetName(std::wstring AName) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterter;
	ThreadName = converterter.to_bytes(AName);
}
#pragma warning (pop)

void ThreadWorker::Start() {
	check(Thread == NULL);
	Thread = new std::thread([this]() { Run(); });
	SetThreadName(Thread, ThreadName.c_str());
}

void ThreadWorker::Terminate() {
	check(Thread != NULL);
	if (Thread != NULL) {
		Stop();
		Thread->join();
		//delete Thread;
		Thread = NULL;
	}
}

};