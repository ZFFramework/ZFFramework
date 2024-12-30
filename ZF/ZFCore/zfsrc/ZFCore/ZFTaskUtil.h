/**
 * @file ZFTaskUtil.h
 * @brief task util
 */

#ifndef _ZFI_ZFTaskUtil_h_
#define _ZFI_ZFTaskUtil_h_

#include "ZFTask.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief wait task by #ZFTimerOnce
 */
zfclass ZFLIB_ZFCore ZFWaitTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFWaitTask, ZFTask)

public:
    /** @brief the wait duration */
    ZFPROPERTY_ASSIGN(zftimet, duration)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(zftimet, duration)
            )

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(ZF_IN ZFResultType resultType);
private:
    zfautoT<ZFTaskId> _implTaskId;
};

// ============================================================
/**
 * @brief async task by #zfasync
 */
zfclass ZFLIB_ZFCore ZFAsyncTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFAsyncTask, ZFTask)

public:
    /** @brief the impl */
    ZFPROPERTY_ASSIGN(ZFListener, impl)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(const ZFListener &, impl)
            )

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(ZF_IN ZFResultType resultType);
private:
    zfautoT<ZFTaskId> _implTaskId;
};

// ============================================================
/**
 * @brief post task by #zfpost
 */
zfclass ZFLIB_ZFCore ZFPostTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFPostTask, ZFTask)

public:
    /** @brief the impl */
    ZFPROPERTY_ASSIGN(ZFListener, impl)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(const ZFListener &, impl)
            )

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(ZF_IN ZFResultType resultType);
private:
    zfautoT<ZFTaskId> _implTaskId;
};

// ============================================================
/**
 * @brief async io task by #zfasyncIOCustom
 */
zfclass ZFLIB_ZFCore ZFAsyncIOCustomTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFAsyncIOCustomTask, ZFTask)

public:
    /** @brief the impl */
    ZFPROPERTY_ASSIGN(ZFListener, impl)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_1(
            ZFMP_IN(const ZFListener &, impl)
            )

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(ZF_IN ZFResultType resultType);
private:
    zfautoT<ZFTaskId> _implTaskId;
};

// ============================================================
/**
 * @brief async io task by #zfasyncIO
 */
zfclass ZFLIB_ZFCore ZFAsyncIOTask : zfextend ZFTask {
    ZFOBJECT_DECLARE(ZFAsyncIOTask, ZFTask)

public:
    /** @brief the impl */
    ZFPROPERTY_ASSIGN(ZFOutput, output)
    /** @brief the impl */
    ZFPROPERTY_ASSIGN(ZFInput, input)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    /** @brief construct with impl */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const ZFOutput &, output)
            , ZFMP_IN(const ZFInput &, input)
            )

public:
    zfoverride
    virtual void taskOnStart(void);
    zfoverride
    virtual void taskOnStop(ZF_IN ZFResultType resultType);
private:
    zfautoT<ZFTaskId> _implTaskId;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTaskUtil_h_
