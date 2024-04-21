/**
 * @file ZFProperty.h
 * @brief property for ZFObject type
 */

#ifndef _ZFI_ZFProperty_h_
#define _ZFI_ZFProperty_h_

#include "ZFPropertyFwd.h"
#include "ZFTypeIdFwd.h"
#include "ZFMethod.h"
#include "ZFAny.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFProperty;
typedef void (*_ZFP_ZFPropertyCallbackEnsureInit)(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *owner
        );
typedef void (*_ZFP_ZFPropertyCallbackDealloc)(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *owner
        );
typedef void (*_ZFP_ZFPropertyMethodCleanup)(ZF_IN const ZFMethod *method);
/**
 * @brief info for a property for ZFObject, see #ZFPROPERTY_RETAIN for more info
 */
zffinal zfclassNotPOD ZFLIB_ZFCore ZFProperty {
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFProperty)

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /**
     * @brief get info about this property
     */
    zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief internal property id, for debug use only
     */
    inline const zfchar *propertyInternalId(void) const {
        return this->_ZFP_ZFProperty_propertyInternalId;
    }
    /**
     * @brief true if this property is registered by #ZFPropertyUserRegisterRetain
     */
    inline zfbool propertyIsUserRegister(void) const {
        return this->_ZFP_ZFProperty_propertyIsUserRegister;
    }
    /**
     * @brief true if this property is registered by #ZFPropertyDynamicRegister
     */
    inline zfbool propertyIsDynamicRegister(void) const {
        return this->_ZFP_ZFProperty_propertyIsDynamicRegister;
    }
    /**
     * @brief see #ZFPropertyDynamicRegister
     */
    inline ZFObject *propertyDynamicRegisterUserData(void) const {
        return this->_ZFP_ZFProperty_propertyDynamicRegisterUserData;
    }
    /**
     * @brief true if the property is serializable
     *
     * property are serializable except:
     * -  #propertyTypeId is #ZFTypeId_none
     * -  assign property with ZFObject type
     * -  #ZFTypeInfo::typeIdSerializable returned false
     *
     * @note this property would be calculated at runtime,
     *   take care of performance
     */
    zfbool propertySerializable(void) const;
    /**
     * @brief get the property's owner class
     */
    inline const ZFClass *propertyOwnerClass(void) const {
        return this->_ZFP_ZFProperty_propertyOwnerClass;
    }
    /**
     * @brief name for the property
     */
    inline const zfchar *propertyName(void) const {
        return this->_ZFP_ZFProperty_name;
    }

    /**
     * @brief type string for the property
     *
     * note the type string is the macro expand from the type,
     * it may or may not be same for same type,
     * usually for debug use only
     */
    inline const zfchar *propertyTypeName(void) const {
        return this->_ZFP_ZFProperty_typeName;
    }
    /**
     * @brief type id string declared in ZFPROPERTY_XXX
     *
     * this value should be ensured the type id for the type or #ZFTypeId_none if no known type,
     * this value is used for property's advanced serialize and copy logic,
     * see #ZFTypeInfo
     * @note for retain property, this value is always the class name of the #propertyClassOfRetainProperty
     */
    inline const zfchar *propertyTypeId(void) const {
        return this->_ZFP_ZFProperty_typeId;
    }
    /**
     * @brief get the getter method
     */
    inline const ZFMethod *setterMethod(void) const {
        return this->_ZFP_ZFProperty_setterMethod;
    }
    /**
     * @brief get the getter method
     */
    inline const ZFMethod *getterMethod(void) const {
        return this->_ZFP_ZFProperty_getterMethod;
    }

    /**
     * @brief true if the property is retain property
     *
     * note that bool property is also a assign property,
     * it also have a getter method named "propertyName"
     */
    inline zfbool propertyIsRetainProperty(void) const {
        return (this->_ZFP_ZFProperty_propertyClassOfRetainProperty != zfnull);
    }
    /**
     * @brief for retain property only, get the retain property's declared class
     *
     * note it's the declared class, property's value may be subclass of it
     */
    inline const ZFClass *propertyClassOfRetainProperty(void) const {
        return this->_ZFP_ZFProperty_propertyClassOfRetainProperty;
    }

public:
    /** @brief see #ZFPropertyCallbackIsValueAccessed */
    ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed;
    /** @brief see #ZFPropertyCallbackIsInitValue */
    ZFPropertyCallbackIsInitValue callbackIsInitValue;
    /** @brief see #ZFPropertyCallbackValueReset */
    ZFPropertyCallbackValueReset callbackValueReset;
    /** @brief see #ZFPropertyUserRegisterAssign */
    ZFPropertyCallbackUserRegisterInitValueSetup callbackUserRegisterInitValueSetup;
    /** @brief see #ZFPropertyDynamicRegister */
    ZFPropertyCallbackDynamicRegisterInitValueGetter callbackDynamicRegisterInitValueGetter;

public:
    /** @cond ZFPrivateDoc */
    ZFProperty(void);
    ~ZFProperty(void);
    /** @endcond */
    zfbool _ZFP_ZFPropertyNeedInit;
    void _ZFP_ZFPropertyInit(
            ZF_IN zfbool propertyIsUserRegister
            , ZF_IN zfbool propertyIsDynamicRegister
            , ZF_IN ZFObject *propertyDynamicRegisterUserData
            , ZF_IN const ZFClass *propertyOwnerClass
            , ZF_IN const zfchar *name
            , ZF_IN const zfchar *typeName
            , ZF_IN const zfchar *typeIdName
            , ZF_IN const ZFMethod *setterMethod
            , ZF_IN const ZFMethod *getterMethod
            , ZF_IN _ZFP_ZFPropertyMethodCleanup setterMethodCleanup
            , ZF_IN _ZFP_ZFPropertyMethodCleanup getterMethodCleanup
            , ZF_IN const ZFClass *propertyClassOfRetainProperty
            );
    ZFProperty *_ZFP_ZFProperty_removeConst(void) const {
        return const_cast<ZFProperty *>(this);
    }
public:
    zfuint _ZFP_ZFProperty_refCount;
    zfchar *_ZFP_ZFProperty_propertyInternalId;
    zfbool _ZFP_ZFProperty_propertyIsUserRegister;
    zfbool _ZFP_ZFProperty_propertyIsDynamicRegister;
    ZFObject *_ZFP_ZFProperty_propertyDynamicRegisterUserData;
    ZFObject *_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper;
    const ZFClass *_ZFP_ZFProperty_propertyOwnerClass;
    ZFSigName _ZFP_ZFProperty_name;
    ZFSigName _ZFP_ZFProperty_typeName;
    ZFSigName _ZFP_ZFProperty_typeId;
    const ZFMethod *_ZFP_ZFProperty_setterMethod;
    const ZFMethod *_ZFP_ZFProperty_getterMethod;
    _ZFP_ZFPropertyMethodCleanup _ZFP_ZFProperty_setterMethodCleanup;
    _ZFP_ZFPropertyMethodCleanup _ZFP_ZFProperty_getterMethodCleanup;
    const ZFClass *_ZFP_ZFProperty_propertyClassOfRetainProperty;
    _ZFP_ZFPropertyCallbackEnsureInit _ZFP_ZFProperty_callbackEnsureInit;
    _ZFP_ZFPropertyCallbackDealloc _ZFP_ZFProperty_callbackDealloc;
    ZFCoreArray<_ZFP_PropLifeCycleData> _ZFP_ZFPropertyLifeCycle_OnInit; // ordered from parent to child
    ZFCoreArray<_ZFP_PropLifeCycleData> _ZFP_ZFPropertyLifeCycle_OnVerify;
    ZFCoreArray<_ZFP_PropLifeCycleData> _ZFP_ZFPropertyLifeCycle_OnAttach;
    ZFCoreArray<_ZFP_PropLifeCycleData> _ZFP_ZFPropertyLifeCycle_OnDetach;
};

// ============================================================
zfclassFwd ZFFilterForZFProperty;
/** @brief see #ZFPropertyGetAll */
extern ZFLIB_ZFCore void ZFPropertyGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret
        , ZF_IN_OPT const ZFFilterForZFProperty *propertyFilter = zfnull
        );
/**
 * @brief get all property currently registered, for debug use only
 */
inline ZFCoreArray<const ZFProperty *> ZFPropertyGetAll(ZF_IN_OPT const ZFFilterForZFProperty *propertyFilter = zfnull) {
    ZFCoreArray<const ZFProperty *> ret;
    ZFPropertyGetAllT(ret, propertyFilter);
    return ret;
}

// ============================================================
extern ZFLIB_ZFCore ZFProperty *_ZFP_ZFPropertyRegister(
        ZF_IN zfbool propertyIsUserRegister
        , ZF_IN zfbool propertyIsDynamicRegister
        , ZF_IN ZFObject *propertyDynamicRegisterUserData
        , ZF_IN const ZFClass *propertyOwnerClass
        , ZF_IN const zfchar *name
        , ZF_IN const zfchar *typeName
        , ZF_IN const zfchar *typeIdName
        , ZF_IN const ZFMethod *setterMethod
        , ZF_IN const ZFMethod *getterMethod
        , ZF_IN _ZFP_ZFPropertyMethodCleanup setterMethodCleanup
        , ZF_IN _ZFP_ZFPropertyMethodCleanup getterMethodCleanup
        , ZF_IN const ZFClass *propertyClassOfRetainProperty
        , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
        , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
        , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
        , ZF_IN ZFPropertyCallbackUserRegisterInitValueSetup callbackUserRegisterInitValueSetup
        , ZF_IN ZFPropertyCallbackDynamicRegisterInitValueGetter callbackDynamicRegisterInitValueGetter
        , ZF_IN _ZFP_ZFPropertyCallbackEnsureInit callbackEnsureInit
        , ZF_IN _ZFP_ZFPropertyCallbackDealloc callbackDealloc
        );
extern ZFLIB_ZFCore void _ZFP_ZFPropertyUnregister(ZF_IN const ZFProperty *propertyInfo);

zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFPropertyRegisterHolder {
public:
    _ZFP_ZFPropertyRegisterHolder(
            ZF_IN zfbool propertyIsUserRegister
            , ZF_IN zfbool propertyIsDynamicRegister
            , ZF_IN ZFObject *propertyDynamicRegisterUserData
            , ZF_IN const ZFClass *propertyOwnerClass
            , ZF_IN const zfchar *name
            , ZF_IN const zfchar *typeName
            , ZF_IN const zfchar *typeIdName
            , ZF_IN const ZFMethod *setterMethod
            , ZF_IN const ZFMethod *getterMethod
            , ZF_IN _ZFP_ZFPropertyMethodCleanup setterMethodCleanup
            , ZF_IN _ZFP_ZFPropertyMethodCleanup getterMethodCleanup
            , ZF_IN const ZFClass *propertyClassOfRetainProperty
            , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
            , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
            , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
            , ZF_IN ZFPropertyCallbackUserRegisterInitValueSetup callbackUserRegisterInitValueSetup
            , ZF_IN ZFPropertyCallbackDynamicRegisterInitValueGetter callbackDynamicRegisterInitValueGetter
            , ZF_IN _ZFP_ZFPropertyCallbackEnsureInit callbackEnsureInit
            , ZF_IN _ZFP_ZFPropertyCallbackDealloc callbackDealloc
            )
    : propertyInfo(_ZFP_ZFPropertyRegister(
        propertyIsUserRegister
        , propertyIsDynamicRegister
        , propertyDynamicRegisterUserData
        , propertyOwnerClass
        , name
        , typeName
        , typeIdName
        , setterMethod
        , getterMethod
        , setterMethodCleanup
        , getterMethodCleanup
        , propertyClassOfRetainProperty
        , callbackIsValueAccessed
        , callbackIsInitValue
        , callbackValueReset
        , callbackUserRegisterInitValueSetup
        , callbackDynamicRegisterInitValueGetter
        , callbackEnsureInit
        , callbackDealloc
        )) {
    }
    ~_ZFP_ZFPropertyRegisterHolder(void) {
        _ZFP_ZFPropertyUnregister(this->propertyInfo);
    }
public:
    ZFProperty *propertyInfo;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProperty_h_

