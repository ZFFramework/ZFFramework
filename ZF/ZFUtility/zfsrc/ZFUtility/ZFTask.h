/**
 * @file ZFTask.h
 * @brief abstract task util
 */

#ifndef _ZFI_ZFTask_h_
#define _ZFI_ZFTask_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief abstract task util
 */
zfclass ZFLIB_ZFUtility ZFTask : zfextend ZFTaskId {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFTask, ZFTaskId)

public:
    /**
     * @brief called when task started
     */
    ZFEVENT(TaskOnStart)
    /**
     * @brief called when task started
     */
    ZFEVENT(TaskOnStop)

public:
    /**
     * @brief start the task
     *
     * impl must call #stop or #notifySuccess or #notifyFail when task done,
     * by any of these methods:
     * -  override #taskOnStart
     * -  attach observer to #EventTaskOnStart
     *
     * the task object would be retained during running
     */
    ZFMETHOD_DECLARE_1(void, start
            , ZFMP_IN_OPT(const ZFListener &, onStop, zfnull)
            )
    zfoverride
    virtual void stop(void) {
        this->stop(ZFResultType::e_Cancel);
    }
    /**
     * @brief stop the task
     */
    ZFMETHOD_DECLARE_1(void, stop
            , ZFMP_IN(ZFResultTypeEnum, resultType)
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
    ZFMETHOD_DECLARE_1(void, notifyFail
            , ZFMP_IN(const zfstring &, errorHint)
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
    ZFPROPERTY_ASSIGN(ZFResultTypeEnum, resultType, ZFResultType::e_Success)
    /**
     * @brief for task impl to store error hint, reset when start
     */
    ZFPROPERTY_ASSIGN(zfstring, errorHint)

public:
    /** @brief called to start task */
    virtual inline void taskOnStart(void) {
    }
    /** @brief called to stop task */
    virtual inline void taskOnStop(void) {
    }

protected:
    /**
     * @brief util constructor to attach custom impl to
     *   #EventTaskOnStart and #EventTaskOnStop
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
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);

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

/**
 * @brief group of #ZFTask, all task run concurrently
 */
zfclass ZFLIB_ZFUtility ZFTaskGroup : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFTaskGroup, ZFTask)

    /** @brief add child */
    ZFMETHOD_DECLARE_1(void, child
            , ZFMP_IN(ZFTask *, child)
            )
    /** @brief util to add child task */
    ZFMETHOD_DECLARE_2(void, child
            , ZFMP_IN(const ZFListener &, implStart)
            , ZFMP_IN_OPT(const ZFListener &, implStop, zfnull)
            )

    /** @brief child count */
    ZFMETHOD_DECLARE_0(zfindex, childCount)
    /** @brief child at index */
    ZFMETHOD_DECLARE_1(zfanyT<ZFTask>, childAt
            , ZFMP_IN(zfindex, index)
            )
    /** @brief child remove at index */
    ZFMETHOD_DECLARE_1(zfautoT<ZFTask>, childRemoveAt
            , ZFMP_IN(zfindex, index)
            )
    /** @brief remove all child */
    ZFMETHOD_DECLARE_0(void, childRemoveAll)

public:
    /** @brief child array */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFArray>, childArray, zfobj<ZFArray>())

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(void);
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
};

/**
 * @brief queue of #ZFTask, all task run sequencely
 */
zfclass ZFLIB_ZFUtility ZFTaskQueue : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFTaskQueue, ZFTask)

    /** @brief add child */
    ZFMETHOD_DECLARE_1(void, child
            , ZFMP_IN(ZFTask *, child)
            )
    /** @brief util to add child task */
    ZFMETHOD_DECLARE_2(void, child
            , ZFMP_IN(const ZFListener &, implStart)
            , ZFMP_IN_OPT(const ZFListener &, implStop, zfnull)
            )

    /** @brief child count */
    ZFMETHOD_DECLARE_0(zfindex, childCount)
    /** @brief child at index */
    ZFMETHOD_DECLARE_1(zfanyT<ZFTask>, childAt
            , ZFMP_IN(zfindex, index)
            )
    /** @brief child remove at index */
    ZFMETHOD_DECLARE_1(zfautoT<ZFTask>, childRemoveAt
            , ZFMP_IN(zfindex, index)
            )
    /** @brief remove all child */
    ZFMETHOD_DECLARE_0(void, childRemoveAll)

public:
    /** @brief child array */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFArray>, childArray, zfobj<ZFArray>())

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(void);
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTask_h_
