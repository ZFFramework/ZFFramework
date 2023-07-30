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
 * for the callback:
 * -  #ZFArgs::param0 is a #v_zfidentity holds running task id,
 *   it's set to zfidentityInvalid if canceled by #zfasyncCancel,
 *   you may check it during thread running
 * -  #ZFArgs::result can be set to store the callback's result,
 *   which would passed to finishCallback as #ZFArgs::param0
 *
 * for the finishCallback:
 * -  #ZFArgs::param0 is the result object passed from callback
 *
 * when #zfasyncCancel called after this method,
 * the callback may or may not be canceled,
 * but finishCallback would be canceled
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfidentity, zfasync
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, ZFCallback())
        )

/**
 * @brief try to cancel the task or finishCallback started by #zfasync
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, zfasyncCancel
        , ZFMP_IN(zfidentity, taskId)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_zfasync_h_

