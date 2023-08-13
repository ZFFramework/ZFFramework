/**
 * @file ZFTaskMap.h
 * @brief task map util
 */

#ifndef _ZFI_ZFTaskMap_h_
#define _ZFI_ZFTaskMap_h_

#include "ZFArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTaskMapPrivate;
/**
 * @brief task map util
 */
zfclass ZFLIB_ZFCore ZFTaskMap : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFTaskMap, ZFObject)

    /** @brief see #ZFTaskMap */
    ZFMETHOD_DECLARE_1(zfidentity, attach
            , ZFMP_IN(ZFObject *, taskData)
            )
    /** @brief see #ZFTaskMap */
    ZFMETHOD_DECLARE_1(zfautoObject, detach
            , ZFMP_IN(zfidentity, taskId)
            )
    /** @brief see #ZFTaskMap */
    ZFMETHOD_DECLARE_0(zfautoObjectT<ZFArray *>, detachAll)
    /** @brief see #ZFTaskMap */
    ZFMETHOD_DECLARE_1(zfautoObject, exist
            , ZFMP_IN(zfidentity, taskId)
            )

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFTaskMapPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTaskMap_h_

