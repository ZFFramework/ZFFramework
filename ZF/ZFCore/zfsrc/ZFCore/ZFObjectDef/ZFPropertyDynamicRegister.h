/**
 * @file ZFPropertyDynamicRegister.h
 * @brief user registered ZFProperty
 */

#ifndef _ZFI_ZFPropertyDynamicRegister_h_
#define _ZFI_ZFPropertyDynamicRegister_h_

#include "ZFMethodDynamicRegister.h"
#include "ZFProperty.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFPropertyDynamicRegisterParam;
/**
 * @brief dynamic register a property,
 *   return null if fail
 *
 * comparing to #ZFPropertyUserRegisterRetain, a dynamic property would:
 * -  completely dynamic, can be registered from script language
 * -  use #ZFMethodGenericInvoker to implement,
 *   have lower performance (trade for flexibility)
 * -  no weak property support (assign property with ZFObject type)
 * -  you may register a property by supplying existing setter and getter method,
 *   or simply leave it empty to use builtin impl
 *
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 */
extern ZFLIB_ZFCore const ZFProperty *ZFPropertyDynamicRegister(
        ZF_IN const ZFPropertyDynamicRegisterParam &param
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief see #ZFPropertyDynamicRegister
 */
extern ZFLIB_ZFCore void ZFPropertyDynamicUnregister(ZF_IN const ZFProperty *property);

// ============================================================
zfclassFwd _ZFP_ZFPropertyDynamicRegisterParamPrivate;
/** @brief param for #ZFPropertyDynamicRegister */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFPropertyDynamicRegisterParam {
public:
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyDynamicRegisterUserData(ZF_IN ZFObject *propertyDynamicRegisterUserData);
    /** @brief see #ZFPropertyDynamicRegister */
    zfany propertyDynamicRegisterUserData(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyOwnerClass(ZF_IN const ZFClass *propertyOwnerClass);
    /** @brief see #ZFPropertyDynamicRegister */
    const ZFClass *propertyOwnerClass(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyTypeId(ZF_IN const zfstring &propertyTypeId);
    /** @brief see #ZFPropertyDynamicRegister */
    const zfstring &propertyTypeId(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyTypeName(ZF_IN const zfstring &propertyTypeName);
    /** @brief see #ZFPropertyDynamicRegister */
    const zfstring &propertyTypeName(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyName(ZF_IN const zfstring &propertyName);
    /** @brief see #ZFPropertyDynamicRegister */
    const zfstring &propertyName(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyClassOfRetainProperty(ZF_IN const ZFClass *propertyClassOfRetainProperty);
    /** @brief see #ZFPropertyDynamicRegister */
    const ZFClass *propertyClassOfRetainProperty(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyInitValueCallback(ZF_IN ZFPropertyCallbackDynamicRegisterInitValueGetter propertyInitValueCallback);
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackDynamicRegisterInitValueGetter propertyInitValueCallback(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertySetterType(ZF_IN ZFMethodPrivilegeType propertySetterType);
    /** @brief see #ZFPropertyDynamicRegister */
    ZFMethodPrivilegeType propertySetterType(void) const;

    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyGetterType(ZF_IN ZFMethodPrivilegeType propertyGetterType);
    /** @brief see #ZFPropertyDynamicRegister */
    ZFMethodPrivilegeType propertyGetterType(void) const;

    // ============================================================
    // for custom impl
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &propertyCustomImpl(
            ZF_IN const ZFMethod *propertySetterMethod
            , ZF_IN const ZFMethod *propertyGetterMethod
            , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
            , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
            , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
            );

    /** @brief see #ZFPropertyDynamicRegister */
    const ZFMethod *propertyCustomImplSetterMethod(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    const ZFMethod *propertyCustomImplGetterMethod(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackIsValueAccessed propertyCustomImplCallbackIsValueAccessed(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackIsInitValue propertyCustomImplCallbackIsInitValue(void) const;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackValueReset propertyCustomImplCallbackValueReset(void) const;

public:
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyDynamicRegisterParam &zfunsafe_disableChecker(ZF_IN zfbool disableChecker);
    /** @brief see #ZFPropertyDynamicRegister */
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

    // ============================================================
public:
    /** @cond ZFPrivateDoc */
    ZFPropertyDynamicRegisterParam(void);
    ZFPropertyDynamicRegisterParam(ZF_IN ZFPropertyDynamicRegisterParam const &ref);
    ~ZFPropertyDynamicRegisterParam(void);
    ZFPropertyDynamicRegisterParam &operator = (ZF_IN ZFPropertyDynamicRegisterParam const &ref);
    zfbool operator == (ZF_IN ZFPropertyDynamicRegisterParam const &ref) const;
    zfbool operator != (ZF_IN ZFPropertyDynamicRegisterParam const &ref) const {return !this->operator == (ref);}
    /** @endcond */

private:
    _ZFP_ZFPropertyDynamicRegisterParamPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDynamicRegister_h_

