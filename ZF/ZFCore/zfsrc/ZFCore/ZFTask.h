/**
 * @file ZFTask.h
 * @brief abstract task util
 */

#ifndef _ZFI_ZFTask_h_
#define _ZFI_ZFTask_h_

#include "ZFResultType.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief abstract task util
 */
zfclass ZFLIB_ZFCore ZFTask : zfextend ZFTaskId {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFTask, ZFTaskId)

public:
    /**
     * @brief called when task started
     */
    ZFEVENT(TaskOnStart)
    /**
     * @brief called when task started
     *
     * param0 is a #ZFResultType to show the result
     */
    ZFEVENT(TaskOnStop)

public:
    /**
     * @brief start the task
     *
     * impl must call #stop or #notifySuccess or #notifyFail when task done,
     * by any of these methods:
     * -  override #taskOnStart
     * -  attach observer to #E_TaskOnStart
     *
     * the task object would be retained during running
     */
    ZFMETHOD_DECLARE_1(void, start
            , ZFMP_IN_OPT(const ZFListener &, onStop, zfnull)
            )
    zfoverride
    virtual void stop(void) {
        this->stop(v_ZFResultType::e_Cancel);
    }
    /**
     * @brief stop the task
     */
    ZFMETHOD_DECLARE_1(void, stop
            , ZFMP_IN(ZFResultType, resultType)
            )
    /**
     * @brief whether task running
     */
    ZFMETHOD_DECLARE_0(zfbool, started)

public:
    /**
     * @brief util method to call stop with success result
     */
    ZFMETHOD_DECLARE_1(void, notifySuccess
            , ZFMP_IN_OPT(ZFObject *, result, zfnull)
            )
    /**
     * @brief util method to call stop with fail result
     */
    ZFMETHOD_DECLARE_2(void, notifyFail
            , ZFMP_IN(const zfstring &, errorHint)
            , ZFMP_IN_OPT(ZFObject *, result, zfnull)
            )

public:
    /**
     * @brief for task impl to store task result, actual result depends on task,
     *   reset to null when start
     */
    ZFPROPERTY_RETAIN(zfany, result)
    /**
     * @brief for task impl to store task result, reset to success when start
     */
    ZFPROPERTY_ASSIGN(ZFResultType, resultType, v_ZFResultType::e_Success)
    /**
     * @brief for task impl to store error hint, reset when start
     */
    ZFPROPERTY_ASSIGN(zfstring, errorHint)

public:
    /** @brief called to start task */
    virtual inline void taskOnStart(void) {
    }
    /** @brief called to stop task */
    virtual inline void taskOnStop(ZF_IN ZFResultType resultType) {
    }

protected:
    /**
     * @brief util constructor to attach custom impl to
     *   #E_TaskOnStart and #E_TaskOnStop
     */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const ZFListener &, implOnStart)
            , ZFMP_IN_OPT(const ZFListener &, implOnStop, zfnull)
            )
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->stop();
        zfsuper::objectOnDeallocPrepare();
    }
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret);

private:
    zfbool _ZFP_started;
    ZFListener _ZFP_onStop;
protected:
    /** @cond ZFPrivateDoc */
    ZFTask(void)
    : _ZFP_started(zffalse)
    , _ZFP_onStop()
    {
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTask_h_

