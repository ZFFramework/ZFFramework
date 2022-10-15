/**
 * @file ZFTestCaseUtil.h
 * @brief test case define
 */

#ifndef _ZFI_ZFTestCaseUtil_h_
#define _ZFI_ZFTestCaseUtil_h_

#include "ZFTestCase.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief run a single test case,
 *   return whether successful running
 *
 * we would create an instance of the test case and running it\n
 * test case had been run can be accessed by second param,
 * may be null if the test case run and stop directly
 */
extern ZFLIB_ZFUtility zfbool ZFTestCaseRun(ZF_IN const ZFClass *cls,
                                            ZF_OUT_OPT ZFTestCase **testCaseHaveRun = zfnull);
/**
 * @brief see #ZFTestCaseRun
 */
extern ZFLIB_ZFUtility zfbool ZFTestCaseRun(ZF_IN const zfchar *classNameFull,
                                            ZF_OUT_OPT ZFTestCase **testCaseHaveRun = zfnull);

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when about to start test case, see #ZFTestCaseRunAllStart
 */
ZFOBSERVER_EVENT_GLOBAL(ZFLIB_ZFUtility, TestCaseRunAllOnStart)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when about to stop test case, see #ZFTestCaseRunAllStop
 */
ZFOBSERVER_EVENT_GLOBAL(ZFLIB_ZFUtility, TestCaseRunAllOnStop)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief use reflect to start all test cases
 *
 * first param can be assigned to specify a callback
 * to check whether a test case can be started
 */
extern ZFLIB_ZFUtility void ZFTestCaseRunAllStart(ZF_IN_OPT const ZFCallbackT<zfbool, ZFTestCase *> *shouldStartChecker = zfnull);
/**
 * @brief stop #ZFTestCaseRunAllStart
 */
extern ZFLIB_ZFUtility void ZFTestCaseRunAllStop(void);

/** @brief see #ZFTestCaseGetAll */
extern ZFLIB_ZFUtility void ZFTestCaseGetAllT(ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret);
/**
 * @brief get all test case currently registered
 */
inline ZFCoreArrayPOD<const ZFClass *> ZFTestCaseGetAll(void)
{
    ZFCoreArrayPOD<const ZFClass *> ret;
    ZFTestCaseGetAllT(ret);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTestCaseUtil_h_

