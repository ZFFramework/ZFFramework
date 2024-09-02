/**
 * @file zfimplLog.h
 * @brief minimal log util for internal debug use only
 */

#ifndef _ZFI_zfimplLog_h_
#define _ZFI_zfimplLog_h_

#include "ZFNamespace.h"

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
                fflush(stdout); \
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
    #define zfimplTimestamp() _ZFP_zfimplTimestamp()
#endif
extern ZFLIB_ZFCore long long _ZFP_zfimplTimestamp(void);

#ifndef zfimplInvokeTimeLogger
    #define zfimplInvokeTimeLogger(hint, ...) \
        _ZFP_zfimplInvokeTimeLogger zfimplUniqueName(ivkTime)(0, hint, ##__VA_ARGS__)
    #define zfimplInvokeTimeLoggerWithCost(cost, hint, ...) \
        _ZFP_zfimplInvokeTimeLogger zfimplUniqueName(ivkTime)(cost, hint, ##__VA_ARGS__)
#endif
class ZFLIB_ZFCore _ZFP_zfimplInvokeTimeLogger {
public:
    _ZFP_zfimplInvokeTimeLogger(int cond, const char *hint, ...);
    ~_ZFP_zfimplInvokeTimeLogger(void);
private:
    char _hint[512];
    long long _lastTime;
    int _cond;
};

// ============================================================
#ifndef zfimplTime
    #define zfimplTime() ((const char *)_ZFP_zfimplTime().buf)
#endif
class ZFLIB_ZFCore _ZFP_zfimplTimeStr {
public:
    char buf[32];
};
extern ZFLIB_ZFCore _ZFP_zfimplTimeStr _ZFP_zfimplTime(void);

/** @endcond */
#endif // #ifndef _ZFI_zfimplLog_h_

