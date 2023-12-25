#pragma once
//#include "UE_CPP_Bridge_Setup.h"

#ifdef P2P_BUILD_FOR_STANDALONE_SERVER

#ifdef _WIN32
  #include "WindowsPlatform.h"
#elif defined(__linux__)
  #include <UnixPlatform.h>
#endif

 //#ifndef verify
 //#define verify(expr)			UE_CHECK_IMPL(expr)
 //#endif

 //#ifndef check
 //#define check(expr)				UE_CHECK_IMPL(expr)
 //#endif
 //
 //	// Technically we could use just the _F version (lambda-based) for asserts
 //	// both with and without formatted messages. However MSVC emits extra
 //	// unnecessary instructions when using a lambda; hence the Exec() impl.
 //	#define UE_CHECK_IMPL(expr) \
 	//		{ \
 	//			if(UNLIKELY(!(expr))) \
 	//			{ \
 	//				struct Impl \
 	//				{ \
 	//					static void FORCENOINLINE UE_DEBUG_SECTION ExecCheckImplInternal() \
 	//					{ \
 	//						FDebug::CheckVerifyFailedImpl(#expr, __FILE__, __LINE__, PLATFORM_RETURN_ADDRESS(), TEXT("")); \
 	//					} \
 	//				}; \
 	//				Impl::ExecCheckImplInternal(); \
 	//				PLATFORM_BREAK_IF_DESIRED(); \
 	//				CA_ASSUME(false); \
 	//			} \
 	//		}


#ifndef check
#define check(expr) \
			if (!(expr)) { \
				PLATFORM_BREAK(); \
			}
#endif

#endif
