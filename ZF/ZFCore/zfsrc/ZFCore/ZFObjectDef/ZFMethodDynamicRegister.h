/**
 * @file ZFMethodDynamicRegister.h
 * @brief user registered ZFMethod
 */

#ifndef _ZFI_ZFMethodDynamicRegister_h_
#define _ZFI_ZFMethodDynamicRegister_h_

#include "ZFMethod.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFMethodDynamicRegisterParam;
zfclassFwd ZFListener;
zfclassFwd ZFMP;
/**
 * @brief dynamic register a method,
 *   return null if fail
 *
 * comparing to #ZFMethodUserRegister_0, a dynamic method would:
 * -  completely dynamic, can be registered from script language
 * -  use #ZFMethodGenericInvoker to implement,
 *   have lower performance (trade for flexibility)
 *
 * @note you may override parent's method by registering a dynamic method in subclass,
 *   at this case, you may use #ZFArgs::callSuper
 *   to call parent's method
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 * @warning (ZFTAG_LIMITATION) for dynamic registered method,
 *   if your method has return value,
 *   you must take care the life cycle of the return value\n
 *   when the method called by plain #ZFMethod::executeExact
 *   instead of #ZFMethod::methodGenericInvoker,
 *   the #ZFTypeIdWrapper created by generic invoker
 *   would be released immediately after method call,
 *   which would cause the raw return value
 *   points to invalid address\n
 *   to solve this, you have these workaround when implementing
 *   #ZFMethodDynamicRegisterParam::methodGenericInvoker:
 *   -  try not to have "non value type" return value for dynamic method
 *     (i.e. reference or pointer type)
 *   -  store return value to owner object as #ZFObject::objectTag
 *     (at this case, you should take care of circular reference,
 *     and value's life cycle)
 *   -  #zfRetain and #zfautoRelease to extend the return value's life time
 *     (at this case, you should ensure the method's caller won't store
 *     the return value for a long time)
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const ZFMethodDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief util to #ZFMethodDynamicRegister
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const ZFClass *ownerClass
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const zfstring &methodName
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_IN_OPT ZFMethodType methodType = ZFMethodTypeVirtual
        , ZF_IN_OPT ZFMethodAccessType methodAccessType = ZFMethodAccessTypePublic
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief util to #ZFMethodDynamicRegister
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodDynamicRegister(
        ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const zfstring &methodName
        , ZF_IN const ZFMP &methodParam
        , ZF_IN const ZFListener &methodImpl
        , ZF_IN_OPT ZFMethodType methodType = ZFMethodTypeVirtual
        , ZF_IN_OPT ZFMethodAccessType methodAccessType = ZFMethodAccessTypePublic
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief see #ZFMethodDynamicRegister
 */
extern ZFLIB_ZFCore void ZFMethodDynamicUnregister(ZF_IN const ZFMethod *method);

// ============================================================
/* ZFMETHOD_MAX_PARAM */
zfclassFwd _ZFP_ZFMethodDynamicRegisterParamPrivate;
/**
 * @brief param for #ZFMethodDynamicRegister
 *
 * required:
 * -  ownerClass or methodNamespace
 * -  methodGenericInvoker or methodImpl
 * -  methodName
 *
 * optional:
 * -  dynamicRegisterUserData, null by default
 * -  methodType, #ZFMethodTypeVirtual by default, auto fallback to #ZFMethodTypeStatic for function type method
 * -  methodAccessType, #ZFMethodAccessTypePublic by default
 * -  returnTypeId, #ZFTypeId_void by default
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFMethodDynamicRegisterParam {
public:
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &dynamicRegisterUserData(ZF_IN ZFObject *dynamicRegisterUserData);
    /** @brief see #ZFMethodDynamicRegister */
    zfany dynamicRegisterUserData(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &ownerClass(ZF_IN const ZFClass *ownerClass);
    /** @brief see #ZFMethodDynamicRegister */
    const ZFClass *ownerClass(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodNamespace(ZF_IN const zfstring &methodNamespace);
    /** @brief see #ZFMethodDynamicRegister */
    const zfstring &methodNamespace(void) const;

    /** @brief see #ZFMethodDynamicRegister, #ZFMethodGenericInvokerParamsCheck */
    ZFMethodDynamicRegisterParam &methodGenericInvoker(ZF_IN ZFMethodGenericInvoker methodGenericInvoker);
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodGenericInvoker methodGenericInvoker(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodImpl(ZF_IN const ZFListener &methodImpl);
    /** @brief see #ZFMethodDynamicRegister */
    const ZFListener &methodImpl(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodType(ZF_IN ZFMethodType methodType);
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodType methodType(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodAccessType(ZF_IN ZFMethodAccessType methodAccessType);
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodAccessType methodAccessType(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodName(ZF_IN const zfstring &methodName);
    /** @brief see #ZFMethodDynamicRegister */
    const zfstring &methodName(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &returnTypeId(ZF_IN const zfstring &returnTypeId);
    /** @brief see #ZFMethodDynamicRegister */
    const zfstring &returnTypeId(void) const;

    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &methodParam(
            ZF_IN const zfstring &paramTypeId
            , ZF_IN_OPT const zfstring &paramName = zfnull
            , ZF_IN_OPT const ZFListener &paramDefaultValueCallback = _ZFP_ZFMethod_paramDefaultValueCallbackDummy()
            );
    /** @brief see #ZFMethodDynamicRegister */
    zfindex paramCount(void) const;
    /** @brief see #ZFMethodDynamicRegister */
    const zfstring &paramTypeIdAt(ZF_IN zfindex index) const;
    /** @brief see #ZFMethodDynamicRegister */
    const zfstring &paramNameAt(ZF_IN zfindex index) const;
    /** @brief see #ZFMethodDynamicRegister */
    const ZFListener &paramDefaultValueCallbackAt(ZF_IN zfindex index) const;

public:
    /** @brief see #ZFMethodDynamicRegister */
    ZFMethodDynamicRegisterParam &zfunsafe_disableChecker(ZF_IN zfbool disableChecker);
    /** @brief see #ZFMethodDynamicRegister */
    zfbool zfunsafe_disableChecker(void) const;

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFMethodDynamicRegisterParam(void);
    ZFMethodDynamicRegisterParam(ZF_IN ZFMethodDynamicRegisterParam const &ref);
    ~ZFMethodDynamicRegisterParam(void);
    ZFMethodDynamicRegisterParam &operator = (ZF_IN ZFMethodDynamicRegisterParam const &ref);
    zfbool operator == (ZF_IN ZFMethodDynamicRegisterParam const &ref) const;
    zfbool operator != (ZF_IN ZFMethodDynamicRegisterParam const &ref) const {return !this->operator == (ref);}
    /** @endcond */

private:
    _ZFP_ZFMethodDynamicRegisterParamPrivate *d;
};

// ============================================================
zfclassFwd _ZFP_ZFMPPrivate;
/** @brief util for #ZFDynamic::method */
zfclassLikePOD ZFLIB_ZFCore ZFMP {
public:
    /** @brief util for #ZFDynamic::method */
    ZFMP &mp(
            ZF_IN const zfstring &paramTypeId
            , ZF_IN_OPT const zfstring &paramName = zfnull
            );
    /** @brief util for #ZFDynamic::method */
    ZFMP &mp(
            ZF_IN const zfstring &paramTypeId
            , ZF_IN const zfstring &paramName
            , ZF_IN ZFObject *paramDefaultValue
            );
    /** @brief util for #ZFDynamic::method */
    ZFMP &mpWithInit(
            ZF_IN const zfstring &paramTypeId
            , ZF_IN const zfstring &paramName
            , ZF_IN const ZFListener &paramDefaultValueCallback
            );

public:
    /** @brief util for #ZFDynamic::method */
    zfindex paramCount(void) const;
    /** @brief util for #ZFDynamic::method */
    const zfstring &paramTypeIdAt(ZF_IN zfindex index) const;
    /** @brief util for #ZFDynamic::method */
    const zfstring &paramNameAt(ZF_IN zfindex index) const;
    /** @brief util for #ZFDynamic::method */
    const ZFListener &paramDefaultValueCallbackAt(ZF_IN zfindex index) const;


public:
    /** @brief see #methodParamListInfo */
    zffinal void methodParamListInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return method param info, like: `P0 p0, P1 p1=def1` */
    zffinal zfstring methodParamListInfo(void) const {
        zfstring ret;
        this->methodParamListInfoT(ret);
        return ret;
    }

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFMP(void);
    ZFMP(ZF_IN const ZFMP &ref);
    ~ZFMP(void);
    ZFMP &operator = (ZF_IN const ZFMP &ref);
    zfbool operator == (ZF_IN const ZFMP &ref) const {return d == ref.d;}
    zfbool operator != (ZF_IN const ZFMP &ref) const {return d != ref.d;}
    /** @endcond */

private:
    _ZFP_ZFMPPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDynamicRegister_h_

