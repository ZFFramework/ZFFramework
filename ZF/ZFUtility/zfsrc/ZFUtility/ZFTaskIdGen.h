/**
 * @file ZFTaskIdGen.h
 * @brief task map util
 */

#ifndef _ZFI_ZFTaskIdGen_h_
#define _ZFI_ZFTaskIdGen_h_

#include "ZFUtilityDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTaskIdGenPrivate;
/**
 * @brief task map util
 */
zfclass ZFLIB_ZFUtility ZFTaskIdGen : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFTaskIdGen, ZFObject)

    /** @brief see #ZFTaskIdGen */
    ZFMETHOD_DECLARE_1(zfidentity, attach
            , ZFMP_IN(ZFObject *, taskData)
            )
    /** @brief see #ZFTaskIdGen */
    ZFMETHOD_DECLARE_1(zfauto, detach
            , ZFMP_IN(zfidentity, taskId)
            )
    /** @brief see #ZFTaskIdGen */
    ZFMETHOD_DECLARE_0(zfautoT<ZFArray>, detachAll)
    /** @brief see #ZFTaskIdGen */
    ZFMETHOD_DECLARE_1(zfauto, exist
            , ZFMP_IN(zfidentity, taskId)
            )

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFTaskIdGenPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTaskIdGen_h_

