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
 * @brief run a single test case
 *
 * we would create an instance of the test case and running it\n
 * return the test case if success and not stopped immediately
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUtility, zfautoT<ZFTestCase *>, ZFTestCaseRun
        , ZFMP_IN(const ZFClass *, cls)
        )
/**
 * @brief see #ZFTestCaseRun
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUtility, zfautoT<ZFTestCase *>, ZFTestCaseRun
        , ZFMP_IN(const zfchar *, classNameFull)
        )

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when about to start test case, see #ZFTestCaseRunAllStart
 */
ZFOBSERVER_EVENT_GLOBAL(TestCaseRunAllOnStart)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when about to stop test case, see #ZFTestCaseRunAllStop
 */
ZFOBSERVER_EVENT_GLOBAL(TestCaseRunAllOnStop)
ZF_NAMESPACE_END(ZFGlobalEvent)

/**
 * @brief use reflect to start all test cases
 *
 * first param can be assigned to specify a callback
 * to check whether a test case can be started
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUtility, void, ZFTestCaseRunAllStart
        , ZFMP_IN_OPT(const ZFCoreArray<const ZFClass *> *, toStart, zfnull)
        )
/**
 * @brief stop #ZFTestCaseRunAllStart
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUtility, void, ZFTestCaseRunAllStop)

/** @brief see #ZFTestCaseGetAll */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFUtility, void, ZFTestCaseGetAllT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFClass *> &, ret)
        )
/**
 * @brief get all test case currently registered
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFUtility, ZFCoreArrayPOD<const ZFClass *>, ZFTestCaseGetAll)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTestCaseUtil_h_

