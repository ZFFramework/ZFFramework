/**
 * @file ZFTaskUtil.h
 * @brief task util
 */

#ifndef _ZFI_ZFTaskUtil_h_
#define _ZFI_ZFTaskUtil_h_

#include "ZFTask.h"
#include "ZFThread_zfasync.h"
#include "ZFThread_zfpost.h"
#include "ZFThread_zfasyncIO.h"
ZF_NAMESPACE_GLOBAL_BEGIN

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
    virtual void taskOnStop(ZF_IN ZFResultTypeEnum resultType);
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
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
    virtual void taskOnStop(ZF_IN ZFResultTypeEnum resultType);
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
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
    virtual void taskOnStop(ZF_IN ZFResultTypeEnum resultType);
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
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
    virtual void taskOnStop(ZF_IN ZFResultTypeEnum resultType);
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
private:
    zfautoT<ZFTaskId> _implTaskId;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTaskUtil_h_