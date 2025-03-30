/**
 * @file ZFPropertyDynamicRegisterExtra.h
 * @brief user registered ZFProperty
 */

#ifndef _ZFI_ZFPropertyDynamicRegisterExtra_h_
#define _ZFI_ZFPropertyDynamicRegisterExtra_h_

#include "ZFPropertyDynamicRegister.h"
#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFListener;

/**
 * @brief property life cycle, see #ZFPROPERTY_ON_INIT_DECLARE
 */
typedef enum {
    ZFPropertyLifeCycleOnInit, /**< @brief see #ZFPROPERTY_ON_INIT_DECLARE */
    ZFPropertyLifeCycleOnUpdate, /**< @brief see #ZFPROPERTY_ON_INIT_DECLARE */
    ZFPropertyLifeCycleOnAttach, /**< @brief see #ZFPROPERTY_ON_INIT_DECLARE */
    ZFPropertyLifeCycleOnDetach, /**< @brief see #ZFPROPERTY_ON_INIT_DECLARE */
} ZFPropertyLifeCycle;

/**
 * @brief dynamic version of #ZFPROPERTY_ON_INIT_DECLARE
 *
 * can not be registered if the property already has custom life cycle for ownerClass
 */
extern ZFLIB_ZFCore zfbool ZFPropertyDynamicRegisterLifeCycle(
        ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *ownerClassOrNull
        , ZF_IN ZFPropertyLifeCycle lifeCycle
        , ZF_IN const ZFListener &callback
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/** @brief see #ZFPropertyDynamicRegisterLifeCycle */
extern ZFLIB_ZFCore zfbool ZFPropertyDynamicUnregisterLifeCycle(
        ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *ownerClassOrNull
        , ZF_IN ZFPropertyLifeCycle lifeCycle
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDynamicRegisterExtra_h_

