/**
 * @file ZFThread_zfasync.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_zfasync_h_
#define _ZFI_ZFThread_zfasync_h_

#include "ZFThread.h"
#include "ZFTaskIdUtil.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to run code asynchronizely
 *
 * callback would run in new thread,
 * finishCallback would run in the same thread that called this method
 */
ZFMETHOD_FUNC_DECLARE_4(zftaskid, zfasync,
                        ZFMP_IN(const ZFListener &, callback),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                        ZFMP_IN_OPT(const ZFListener &, finishCallback, ZFCallback()),
                        ZFMP_IN_OPT(ZFObject *, finishCallbackUserData, zfnull))

/**
 * @brief try to cancel the task or finishCallback started by #zfasync
 */
ZFMETHOD_FUNC_DECLARE_1(void, zfasyncCancel,
                        ZFMP_IN_OUT(zftaskid &, taskid))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_zfasync_h_

