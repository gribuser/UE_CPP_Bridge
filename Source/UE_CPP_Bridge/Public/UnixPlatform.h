#pragma once

#include <linux/version.h>
#include <signal.h>

#define PLATFORM_LITTLE_ENDIAN								1

// Function type macros.
#define VARARGS															/* Functions with variable arguments */
#define CDECL															/* Standard C function */
#define STDCALL															/* Standard calling convention */
#if UE_BUILD_DEBUG
#define FORCEINLINE inline 											/* Don't force code to be inline, or you'll run into -Wignored-attributes */
#else
#define FORCEINLINE inline __attribute__ ((always_inline))			/* Force code to be inline */
#endif // UE_BUILD_DEBUG
#define FORCENOINLINE __attribute__((noinline))							/* Force code to NOT be inline */
#define FUNCTION_CHECK_RETURN_END __attribute__ ((warn_unused_result))	/* Wrap a function signature in this to warn that callers should not ignore the return value. */
#define FUNCTION_NO_RETURN_END __attribute__ ((noreturn))				/* Wrap a function signature in this to indicate that the function never returns. */

// DLL export and import definitions
#define DLLEXPORT			__attribute__((visibility("default")))
#define DLLIMPORT			__attribute__((visibility("default")))

#if PLATFORM_CPU_X86_FAMILY
#define PLATFORM_BREAK()							__asm__ volatile("int $0x03")
#else
#define PLATFORM_BREAK()							raise(SIGTRAP)
#endif // PLATFORM_CPU_X86_FAMILY
