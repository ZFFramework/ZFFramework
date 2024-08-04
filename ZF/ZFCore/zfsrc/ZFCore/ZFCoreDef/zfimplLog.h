/**
 * @file zfimplLog.h
 * @brief minimal log util for internal debug use only
 */

#ifndef _ZFI_zfimplLog_h_
#define _ZFI_zfimplLog_h_
/** @cond ZFPrivateDoc */

#ifndef zfimplLog
    #if (defined(ANDROID) || defined(__ANDROID__))
        #include <jni.h>
        #include <android/log.h>
        #define zfimplLog(fmt, ...) \
            ((void)__android_log_print(ANDROID_LOG_ERROR, "JNILog", fmt, ##__VA_ARGS__))
    #elif (defined(QT_VERSION) || defined(QT_CORE_LIB))
        #include <QDebug>
        #define zfimplLog(fmt, ...) \
            qDebug(fmt, ##__VA_ARGS__)
    #else
        #include <cstdio>
        #define zfimplLog(fmt, ...) \
            do { \
                printf(fmt, ##__VA_ARGS__); \
                printf("\n"); \
            } while(false)
    #endif
#endif

#ifndef zfimplUniqueName
    #define zfimplUniqueName(name) _ZFP_zfimplUniqueName2(name, __LINE__)
    #define _ZFP_zfimplUniqueName2(name, line) _ZFP_zfimplUniqueName3(name, line)
    #define _ZFP_zfimplUniqueName3(name, line) _ZFP_uniq_##name##_##line
#endif

// ============================================================
#ifndef zfimplTimestamp
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
    inline long long _ZFP_zfimplTimestamp(void) {
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
    #define zfimplTimestamp() _ZFP_zfimplTimestamp()
#endif

#ifndef zfimplInvokeTimeLogger
    #include <stdio.h>
    class _ZFP_zfimplInvokeTimeLogger {
    public:
        _ZFP_zfimplInvokeTimeLogger(int cond, const char *hint, ...)
        : _cond(cond)
        {
            va_list vaList;
            va_start(vaList, hint);
            vsnprintf(_hint, sizeof(_hint), hint, vaList);
            va_end(vaList);

            _lastTime = zfimplTimestamp();
        }
        ~_ZFP_zfimplInvokeTimeLogger(void)
        {
            int cost = (int)(zfimplTimestamp() - _lastTime);
            if(cost >= _cond) {
                zfimplLog("%4d : %s", (int)(zfimplTimestamp() - _lastTime), _hint);
            }
        }
    private:
        char _hint[512];
        long long _lastTime;
        int _cond;
    };
    #define zfimplInvokeTimeLogger(hint, ...) \
        _ZFP_zfimplInvokeTimeLogger zfimplUniqueName(ivkTime)(0, hint, ##__VA_ARGS__)
    #define zfimplInvokeTimeLoggerWithCost(cost, hint, ...) \
        _ZFP_zfimplInvokeTimeLogger zfimplUniqueName(ivkTime)(cost, hint, ##__VA_ARGS__)
#endif

// ============================================================
#ifndef zfimplTime
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
    class _ZFP_zfimplTimeStr {
    public:
        char *buf;
    public:
        _ZFP_zfimplTimeStr(void) : buf((char *)malloc(32)) {
            buf[0] = '\0';
        }
        ~_ZFP_zfimplTimeStr(void) {
            if(buf) {
                free(buf);
            }
        }
    };
    inline _ZFP_zfimplTimeStr _ZFP_zfimplTime(void) {
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
        struct tm *tm = gmtime(&sec);
        if(tm == NULL) {
            return ret;
        }
        snprintf(ret.buf, 32, "%02d:%02d:%02d.%03d"
                , (int)tm->tm_hour
                , (int)tm->tm_min
                , (int)tm->tm_sec
                , (int)(usec / 1000)
                );
        return ret;
    }
    #define zfimplTime() _ZFP_zfimplTime().buf
#endif

/** @endcond */
#endif // #ifndef _ZFI_zfimplLog_h_

