#include "zfimplLog.h"
#include <cstdarg>

// ============================================================
#include <ctime>
#if defined(_WIN32) || defined(WIN32)
    #include <Windows.h>
#elif __APPLE__
    #include <sys/time.h>
    #include <mach/mach.h>
    #include <mach/mach_time.h>
#else
    // need -lrt for Posix
    #include <sys/time.h>
#endif
long long _ZFP_zfimplTimestamp(void) {
    #if defined(_WIN32) || defined(WIN32)
        return (long long)GetTickCount();
    #elif __APPLE__
        static mach_timebase_info_data_t _timebaseInfo;
        if(_timebaseInfo.denom == 0) {
            (void)mach_timebase_info(&_timebaseInfo);
        }
        return (long long)(((mach_absolute_time() / 1000000) * _timebaseInfo.numer) / _timebaseInfo.denom);
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (long long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    #endif
}

#include <stdio.h>
_ZFP_zfimplInvokeTimeLogger::_ZFP_zfimplInvokeTimeLogger(int cond, const char *hint, ...)
: _cond(cond)
{
    va_list vaList;
    va_start(vaList, hint);
    vsnprintf(_hint, sizeof(_hint), hint, vaList);
    va_end(vaList);

    _lastTime = zfimplTimestamp();
}
_ZFP_zfimplInvokeTimeLogger::~_ZFP_zfimplInvokeTimeLogger(void) {
    int cost = (int)(zfimplTimestamp() - _lastTime);
    if(cost >= _cond) {
        zfimplLog("%4d : %s", (int)(zfimplTimestamp() - _lastTime), _hint);
    }
}

// ============================================================
#include <ctime>
#include <cstdio>
#include <cstdlib>
#if defined(_WIN32) || defined(WIN32)
    #include <Windows.h>
#elif __APPLE__
    #include <sys/time.h>
    #include <mach/mach.h>
    #include <mach/mach_time.h>
#else
    // need -lrt for Posix
    #include <sys/time.h>
#endif
_ZFP_zfimplTimeStr _ZFP_zfimplTime(void) {
    #if defined(_WIN32) || defined(WIN32)
        union {
            unsigned short ns100;
            FILETIME ft;
        } now;
        GetSystemTimeAsFileTime(&now.ft);
        time_t sec = (time_t)((now.ns100 - 116444736000000000LL) / 10000000LL);
        time_t usec = (time_t)((now.ns100 / 10LL) % 1000000LL);
    #else
        struct timeval tvTmp;
        gettimeofday(&tvTmp, NULL);
        time_t sec = tvTmp.tv_sec;
        time_t usec = tvTmp.tv_usec;
    #endif

    _ZFP_zfimplTimeStr ret;
    ret.buf[0] = '\0';
    struct tm *tm = gmtime(&sec);
    if(tm == NULL) {
        return ret;
    }
    snprintf(ret.buf, sizeof(ret.buf), "%02d:%02d:%02d.%03d"
            , (int)tm->tm_hour
            , (int)tm->tm_min
            , (int)tm->tm_sec
            , (int)(usec / 1000)
            );
    return ret;
}

