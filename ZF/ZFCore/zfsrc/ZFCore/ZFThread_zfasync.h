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
 * finishCallback would run in #ZFThread::mainThread\n
 * \n
 * for the callback:
 * -  #ZFArgs::sender is the taskId returned from #zfasync
 * -  #ZFArgs::param0 is a #ZFObject holds running task id,
 *   it's set to null if canceled,
 *   you may check it during thread running
 * -  #ZFArgs::result can be set to store the callback's result,
 *   which would passed to finishCallback as #ZFArgs::param0
 *
 * for the finishCallback:
 * -  #ZFArgs::sender is the taskId returned from #zfasync
 * -  #ZFArgs::param0 is the result object passed from callback
 *
 * when #ZFTaskId::stop called after this method,
 * the callback may or may not be canceled,
 * but finishCallback would be canceled
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfautoT<ZFTaskId>, zfasync
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_zfasync_h_

