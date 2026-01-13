/**
 * @file ZFThread_zfasyncIO.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_zfasyncIO_h_
#define _ZFI_ZFThread_zfasyncIO_h_

#include "ZFThreadPool.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util to perform IO in #ZFThreadPoolForIO
 *
 * callback would run in new thread,
 * finishCallback would run in #ZFThread::mainThread\n
 * \n
 * for the callback:
 * -  #ZFArgs::sender is the taskId returned from #zfasyncIO
 * -  #ZFArgs::param0 is a #ZFObject holds running task id,
 *   it's set to null if canceled,
 *   you may check it during thread running
 * -  #ZFArgs::result can be set to store the callback's result,
 *   which would passed to finishCallback as #ZFArgs::param0
 *
 * for the finishCallback:
 * -  #ZFArgs::sender is the taskId returned from #zfasyncIO
 * -  #ZFArgs::param0 is the result object passed from callback
 *
 * when #ZFTaskId::stop called after this method,
 * the callback may or may not be canceled,
 * but finishCallback would be canceled
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfautoT<ZFTaskId>, zfasyncIOCustom
        , ZFMP_IN(const ZFListener &, callback)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        )

/**
 * @brief resumable state for #zfasyncIO
 */
zfclass ZFAsyncIOResumable : zfextend ZFObject, zfimplement ZFSerializable {
    ZFOBJECT_DECLARE(ZFAsyncIOResumable, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable)

public:
    /** @cond ZFPrivateDoc */
    ZFPROPERTY_ASSIGN(zfstring, outputId)
    ZFPROPERTY_ASSIGN(zfstring, inputId)
    ZFPROPERTY_ASSIGN(zfindex, totalSize)
    ZFPROPERTY_ASSIGN(zfindex, blockSize)
    ZFPROPERTY_RETAIN(zfany, successBlocks)
    /** @endcond */

    /** @brief reset state */
    ZFMETHOD_DECLARE_0(void, reset)
};

/**
 * @brief util to perform IO in #ZFThreadPoolForIO
 *
 * for the finishCallback:
 * -  #ZFArgs::sender is the taskId returned from #zfasyncIO
 * -  #ZFArgs::param0 is a #v_zfindex indicates size written, or zfindexMax if failed
 * -  #ZFArgs::param1 is the resumable object
 *
 * for the progressCallback:
 * -  #ZFArgs::sender is the taskId returned from #zfasyncIO
 * -  #ZFArgs::param0 is a #v_zfindex indicates size written
 * -  #ZFArgs::param1 is a #v_zfindex indicates total size, or zfindexMax if unknown
 *
 * when #ZFTaskId::stop called after this method,
 * the callback may or may not be canceled,
 * but finishCallback would be canceled
 *
 * if you want to make the IO resumable:
 * -  input and output must support #ZFIOCallback::ioSeek
 * -  first time:
 *   -  pass a non-null resumable
 *   -  when stopped failed or canceled, do not remove output file,
 *     and store or serialize the resumable object
 * -  to resume state:
 *   -  input and output's #ZFCallback::callbackId must be same with first time
 *   -  open output as #v_ZFIOOpenOption::e_Modify
 *     (you must ensure the previous contents was kept)
 *   -  restore or serialize the resumable object
 */
ZFMETHOD_FUNC_DECLARE_5(ZFLIB_ZFCore, zfautoT<ZFTaskId>, zfasyncIO
        , ZFMP_IN(ZF_IN const ZFOutput &, output)
        , ZFMP_IN(ZF_IN const ZFInput &, input)
        , ZFMP_IN_OPT(const ZFListener &, finishCallback, zfnull)
        , ZFMP_IN_OPT(const ZFListener &, progressCallback, zfnull)
        , ZFMP_IN_OPT(ZFAsyncIOResumable *, resumable, zfnull)
        )

// ============================================================
/**
 * @brief the thread pool for #zfasyncIO
 */
zfclass ZFLIB_ZFCore ZFThreadPoolForIO : zfextend ZFThreadPool {
    ZFOBJECT_DECLARE(ZFThreadPoolForIO, ZFThreadPool)
    ZFOBJECT_SINGLETON_DECLARE(ZFThreadPoolForIO, instance)

    /**
     * @brief when input size greater than this,
     *   try to split task to different thread
     */
    ZFPROPERTY_ASSIGN(zfindex, blockSize, 4 * 1024 * 1024)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_zfasyncIO_h_

