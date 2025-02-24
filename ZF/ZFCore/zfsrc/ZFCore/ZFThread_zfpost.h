/**
 * @file ZFThread_zfpost.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_zfpost_h_
#define _ZFI_ZFThread_zfpost_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to run code in specified thread or #ZFThread::mainThread if not specified
 *   or not #ZFThread::taskQueueAvailable
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfautoT<ZFTaskId>, zfpost
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(ZFThread *, target, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_zfpost_h_

