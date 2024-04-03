// Hacked by PJ. Copy from llvm-config.h.cmake

/* This file enumerates variables from the LLVM configuration so that they
   can be in exported headers and won't override package specific directives.
   This is a C header that can be included in the llvm-c headers. */

#ifndef LLVM_CONFIG_H
#define LLVM_CONFIG_H

/* Define if LLVM_ENABLE_DUMP is enabled */
#define LLVM_ENABLE_DUMP 0

/* Target triple LLVM will generate code for by default */
/* Doesn't use `cmakedefine` because it is allowed to be empty. */
#define LLVM_DEFAULT_TARGET_TRIPLE "${LLVM_DEFAULT_TARGET_TRIPLE}"

/* Define if threads enabled */
#define LLVM_ENABLE_THREADS 1

/* Has gcc/MSVC atomic intrinsics */
#define LLVM_HAS_ATOMICS 1

/* Host triple LLVM will be executed on */
#define LLVM_HOST_TRIPLE "${LLVM_HOST_TRIPLE}"

/* LLVM architecture name for the native architecture, if available */
#define LLVM_NATIVE_ARCH ${LLVM_NATIVE_ARCH}

///* LLVM name for the native AsmParser init function, if available */
//#define LLVM_NATIVE_ASMPARSER LLVMInitialize${LLVM_NATIVE_ARCH}AsmParser
//
///* LLVM name for the native AsmPrinter init function, if available */
//#define LLVM_NATIVE_ASMPRINTER LLVMInitialize${LLVM_NATIVE_ARCH}AsmPrinter
//
///* LLVM name for the native Disassembler init function, if available */
//#define LLVM_NATIVE_DISASSEMBLER LLVMInitialize${LLVM_NATIVE_ARCH}Disassembler
//
///* LLVM name for the native Target init function, if available */
//#define LLVM_NATIVE_TARGET LLVMInitialize${LLVM_NATIVE_ARCH}Target
//
///* LLVM name for the native TargetInfo init function, if available */
//#define LLVM_NATIVE_TARGETINFO LLVMInitialize${LLVM_NATIVE_ARCH}TargetInfo
//
///* LLVM name for the native target MC init function, if available */
//#define LLVM_NATIVE_TARGETMC LLVMInitialize${LLVM_NATIVE_ARCH}TargetMC
//
///* LLVM name for the native target MCA init function, if available */
//#define LLVM_NATIVE_TARGETMCA LLVMInitialize${LLVM_NATIVE_ARCH}TargetMCA
//
///* Define if this is Unixish platform */
//#define LLVM_ON_UNIX ${LLVM_ON_UNIX}
//
///* Define if we have the Intel JIT API runtime support library */
//#define LLVM_USE_INTEL_JITEVENTS 0
//
///* Define if we have the oprofile JIT-support library */
//#define LLVM_USE_OPROFILE 0
//
///* Define if we have the perf JIT-support library */
//#define LLVM_USE_PERF 0
//
///* Major version of the LLVM API */
//#define LLVM_VERSION_MAJOR ${LLVM_VERSION_MAJOR}
//
///* Minor version of the LLVM API */
//#define LLVM_VERSION_MINOR ${LLVM_VERSION_MINOR}
//
///* Patch version of the LLVM API */
//#define LLVM_VERSION_PATCH ${LLVM_VERSION_PATCH}
//
///* LLVM version string */
//#define LLVM_VERSION_STRING "${PACKAGE_VERSION}"
//
///* Whether LLVM records statistics for use with GetStatistics(),
// * PrintStatistics() or PrintStatisticsJSON()
// */
//#define LLVM_FORCE_ENABLE_STATS 0
//
///* Define if we have z3 and want to build it */
//#define LLVM_WITH_Z3 ${LLVM_WITH_Z3}
//
///* Define if we have curl and want to use it */
//#define LLVM_ENABLE_CURL ${LLVM_ENABLE_CURL}
//
///* Define if we have cpp-httplib and want to use it */
//#define LLVM_ENABLE_HTTPLIB ${LLVM_ENABLE_HTTPLIB}
//
///* Define if zlib compression is available */
//#define LLVM_ENABLE_ZLIB 0
//
///* Define if zstd compression is available */
//#define LLVM_ENABLE_ZSTD 0
//
///* Define if LLVM is using tflite instead of libtensorflow */
//#define LLVM_HAVE_TFLITE 0
//
///* Define to 1 if you have the <sysexits.h> header file. */
//#define HAVE_SYSEXITS_H ${HAVE_SYSEXITS_H}
//
///* Define if the xar_open() function is supported on this platform. */
//#define LLVM_HAVE_LIBXAR ${LLVM_HAVE_LIBXAR}
//
///* Define if building libLLVM shared library */
//#define LLVM_BUILD_LLVM_DYLIB
//
///* Define if building LLVM with BUILD_SHARED_LIBS */
//#define LLVM_BUILD_SHARED_LIBS
//
///* Define if building LLVM with LLVM_FORCE_USE_OLD_TOOLCHAIN_LIBS */
//#define LLVM_FORCE_USE_OLD_TOOLCHAIN ${LLVM_FORCE_USE_OLD_TOOLCHAIN}
//
///* Define if llvm_unreachable should be optimized with undefined behavior
// * in non assert builds */
//#define LLVM_UNREACHABLE_OPTIMIZE 0

/* Define to 1 if you have the DIA SDK installed, and to 0 if you don't. */
#define LLVM_ENABLE_DIA_SDK 0

///* Define if plugins enabled */
//#define LLVM_ENABLE_PLUGINS

#endif
