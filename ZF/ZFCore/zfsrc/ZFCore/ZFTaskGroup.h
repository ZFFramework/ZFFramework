/**
 * @file ZFTaskGroup.h
 * @brief abstract task util
 */

#ifndef _ZFI_ZFTaskGroup_h_
#define _ZFI_ZFTaskGroup_h_

#include "ZFTask.h"
#include "ZFArray.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief group of #ZFTask, all task run concurrently
 */
zfclass ZFLIB_ZFCore ZFTaskGroup : zfextend ZFTask {
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
    /** @brief util to add #ZFWaitTask */
    ZFMETHOD_DECLARE_1(void, wait
            , ZFMP_IN(zftimet, duration)
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
    virtual void taskOnStop(ZF_IN ZFResultType resultType);
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTaskGroup_h_

