/**
 * @file ZFMethodDynamicRegisterExtra.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodDynamicRegisterExtra_h_
#define _ZFI_ZFMethodDynamicRegisterExtra_h_

#include "ZFMethodDynamicRegister.h"
#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief data to hold params for #ZFMethodGenericInvoker */
zfclass ZFLIB_ZFCore ZFMethodInvokeData : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFMethodInvokeData, ZFObject)

public:
    zfbool invokeSuccess; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    const ZFMethod *invokerMethod; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfany invokerObject; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfstring errorHint; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfauto ret; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    /* ZFMETHOD_MAX_PARAM */
    zfauto param0; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfauto param1; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfauto param2; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfauto param3; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfauto param4; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfauto param5; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfauto param6; /**< @brief data to hold params for #ZFMethodGenericInvoker */
    zfauto param7; /**< @brief data to hold params for #ZFMethodGenericInvoker */

public:
    /**
     * @brief for dynamic registered method to call parent's method
     */
    zffinal zfauto callSuper(void);

    /**
     * @brief util to access param by index
     */
    zffinal const zfauto &paramAt(ZF_IN zfindex index);

    /**
     * @brief util to set param by index
     */
    zffinal ZFMethodInvokeData *paramSet(ZF_IN zfindex index, ZF_IN ZFObject *param);

protected:
    /** @cond ZFPrivateDoc */
    ZFMethodInvokeData(void)
    : ZFObject()
    , invokeSuccess(zftrue)
    , invokerMethod(zfnull)
    , invokerObject(zfnull)
    , errorHint()
    , ret()
    , param0()
    , param1()
    , param2()
    , param3()
    , param4()
    , param5()
    , param6()
    , param7()
    {
    }
    /** @endcond */
public:
    zfoverride
    virtual zfbool objectIsInternal(void) {
        return zftrue;
    }
    zfoverride
    virtual zfbool objectIsInternalPrivate(void) {
        return zftrue;
    }
protected:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDynamicRegisterExtra_h_

