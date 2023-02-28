/**
 * @file ZFThread_zfasync.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_zfasync_h_
#define _ZFI_ZFThread_zfasync_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to run code asynchronizely
 *
 * callback would run in new thread,
 * finishCallback would run in the same thread that called this method\n
 * \n
 * you may store result to callback's #ZFArgs::result,
 * and obtain the result from finishCallback's #ZFArgs::param0
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfidentity, zfasync,
                        ZFMP_IN(const ZFListener &, callback),
                        ZFMP_IN_OPT(const ZFListener &, finishCallback, ZFCallback()))

/**
 * @brief try to cancel the task or finishCallback started by #zfasync
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, zfasyncCancel,
                        ZFMP_IN(zfidentity, taskId))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_zfasync_h_

