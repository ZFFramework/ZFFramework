/**
 * @file ZFCoreStatistic_ZFTime.h
 * @brief util to log function's invoke time
 */

#ifndef _ZFI_ZFCoreStatistic_ZFTime_h_
#define _ZFI_ZFCoreStatistic_ZFTime_h_

#include "ZFTime.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFCoreStatistic)

// ============================================================
/**
 * @brief util to log function's invoke time
 *
 * usage:
 * @code
 *   ZFCoreStatistic::invokeTimeLogBegin(key);
 *   yourHeavyFunc();
 *   ZFCoreStatistic::invokeTimeLogEnd(key);
 *   // ...
 *   // you may call any times
 *
 *   // finally, print result, and remove
 *   ZFLogTrim() << ZFCoreStatistic::invokeTimeGetSummary(key);
 *   ZFCoreStatistic::invokeTimeRemove(key);
 *
 *   // or, you may use the convenient macro to log and output for one time
 *   {
 *       ZFCoreStatisticInvokeTimeLoggerOneTime(key, optional_outputCallback);
 *       yourHeavyFunc();
 *   }
 * @endcode
 */
extern ZFLIB_ZFCore void invokeTimeLogBegin(ZF_IN const zfstring &key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZFLIB_ZFCore void invokeTimeLogEnd(ZF_IN const zfstring &key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZFLIB_ZFCore void invokeTimeRemove(ZF_IN const zfstring &key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZFLIB_ZFCore void invokeTimeRemoveAll(void);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZFLIB_ZFCore zfindex invokeTimeGetInvokeCount(ZF_IN const zfstring &key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZFLIB_ZFCore ZFTimeValue invokeTimeGetAverageTime(ZF_IN const zfstring &key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZFLIB_ZFCore ZFTimeValue invokeTimeGetTotalTime(ZF_IN const zfstring &key);
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
extern ZFLIB_ZFCore void invokeTimeGetSummary(
        ZF_OUT zfstring &ret
        , ZF_IN const zfstring &key
        );
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
inline zfstring invokeTimeGetSummary(ZF_IN const zfstring &key) {
    zfstring ret;
    ZFCoreStatistic::invokeTimeGetSummary(ret, key);
    return ret;
}

// ============================================================
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFCoreStatisticInvokeTimeLogger {
public:
    _ZFP_ZFCoreStatisticInvokeTimeLogger(ZF_IN const zfstring &key)
    : key(key)
    {
        ZFCoreStatistic::invokeTimeLogBegin(key);
    }
    ~_ZFP_ZFCoreStatisticInvokeTimeLogger(void) {
        ZFCoreStatistic::invokeTimeLogEnd(key);
    }
private:
    zfstring key;
};
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
#define ZFCoreStatisticInvokeTimeLogger(key) \
    ZFCoreStatistic::_ZFP_ZFCoreStatisticInvokeTimeLogger ZFUniqueName(ZFCoreStatisticInvokeTimeLogger_v)(key)

// ============================================================
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFCoreStatisticInvokeTimeLoggerOneTime {
public:
    _ZFP_ZFCoreStatisticInvokeTimeLoggerOneTime(
            ZF_IN const zfstring &key
            , ZF_IN const ZFOutput &output = ZFOutputDefault()
            )
    : key(key)
    , output(output)
    {
        ZFCoreStatistic::invokeTimeLogBegin(key);
    }
    ~_ZFP_ZFCoreStatisticInvokeTimeLoggerOneTime(void) {
        ZFCoreStatistic::invokeTimeLogEnd(key);
        output << ZFCoreStatistic::invokeTimeGetSummary(key) << "\n";
        ZFCoreStatistic::invokeTimeRemove(key);
    }
private:
    zfstring key;
    ZFOutput output;
};
/** @brief see #ZFCoreStatistic::invokeTimeLogBegin */
#define ZFCoreStatisticInvokeTimeLoggerOneTime(key, ...) \
    ZFCoreStatistic::_ZFP_ZFCoreStatisticInvokeTimeLoggerOneTime \
        ZFUniqueName(ZFCoreStatisticInvokeTimeLoggerOneTime_v)(key, ##__VA_ARGS__)

ZF_NAMESPACE_END(ZFCoreStatistic)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreStatistic_ZFTime_h_

