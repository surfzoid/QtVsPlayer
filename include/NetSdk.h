#ifndef _NETSDK_H_
#define _NETSDK_H_
#if defined(__APPLE__)
    #include"HCNetSDK.h"
#elif defined(__linux__)
    #include"HCNetSDK.h"
#elif defined(ANDROID)
    #include"HCNetSDK.h"
#elif defined(WIN32)
    #include"HCNetSDKwin.h"
#endif


#endif //_NETSDK_H_
