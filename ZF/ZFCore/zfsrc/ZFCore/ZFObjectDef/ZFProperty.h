/**
 * @file ZFProperty.h
 * @brief property for ZFObject type
 */

#ifndef _ZFI_ZFProperty_h_
#define _ZFI_ZFProperty_h_

#include "ZFPropertyFwd.h"
#include "ZFMethod.h"
#include "zfany.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFProperty;
typedef void (*_ZFP_ZFPropertyCallbackEnsureInit)(
        ZF_IN const ZFProperty *property
        , ZF_IN zfany const &owner
        );
typedef void (*_ZFP_ZFPropertyCallbackDealloc)(
        ZF_IN const ZFProperty *property
        , ZF_IN zfany const &owner
        );
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
    inline zfidentity propertyId(void) const {
        return _propertyId.sigId();
    }
    /**
     * @brief true if this property is registered by #ZFPropertyUserRegisterRetain
     */
    inline zfbool isUserRegister(void) const {
        return ZFBitTest(_stateFlags, _stateFlags_isUserRegister);
    }
    /**
     * @brief true if this property is registered by #ZFPropertyDynamicRegister
     */
    inline zfbool isDynamicRegister(void) const {
        return ZFBitTest(_stateFlags, _stateFlags_isDynamicRegister);
    }
    /**
     * @brief see #ZFPropertyDynamicRegister
     */
    inline zfany dynamicRegisterUserData(void) const {
        return _dynamicRegisterUserData;
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
    inline const ZFClass *ownerClass(void) const {
        return _ownerClass;
    }
    /**
     * @brief name for the property
     */
    inline const zfstring &propertyName(void) const {
        return _propertyName;
    }

    /**
     * @brief type string for the property
     *
     * note the type string is the macro expand from the type,
     * it may or may not be same for same type,
     * usually for debug use only
     */
    inline const zfstring &propertyTypeName(void) const {
        return _propertyTypeName ? _propertyTypeName : _propertyTypeId;
    }
    /**
     * @brief type id string declared in ZFPROPERTY_XXX
     *
     * this value should be ensured the type id for the type or #ZFTypeId_none if no known type,
     * this value is used for property's advanced serialize and copy logic,
     * see #ZFTypeInfo
     * @note for retain property, this value is always the class name of the #propertyClassOfRetainProperty
     */
    inline const zfstring &propertyTypeId(void) const {
        return _propertyTypeId;
    }
    /**
     * @brief get the getter method
     */
    inline const ZFMethod *setterMethod(void) const {
        return _setterMethod;
    }
    /**
     * @brief get the getter method
     */
    inline const ZFMethod *getterMethod(void) const {
        return _getterMethod;
    }

    /**
     * @brief true if the property is retain property
     *
     * note that bool property is also a assign property,
     * it also have a getter method named "propertyName"
     */
    inline zfbool isRetainProperty(void) const {
        return (_propertyClassOfRetainProperty != zfnull);
    }
    /**
     * @brief for retain property only, get the retain property's declared class
     *
     * note it's the declared class, property's value may be subclass of it
     */
    inline const ZFClass *propertyClassOfRetainProperty(void) const {
        return _propertyClassOfRetainProperty;
    }

public:
    /**
     * @brief whether the property is internal property
     *
     * properties would be treated as internal if:
     * -  property name starts with '_'
     * -  getter method is not public
     *
     * internal properties would be ignored from serialization,
     * see also #isInternalPrivate
     */
    inline zfbool isInternal(void) const {
        return ZFBitTest(_stateFlags, _stateFlags_isInternal);
    }
    /**
     * @brief whether the property is internal private property
     *
     * properties would be treated as internal private if:
     * -  property name starts with '_ZFP_'
     * -  getter method is private
     *
     * internal private properties would be ignored from serialization and reflection,
     * see also #isInternal
     */
    inline zfbool isInternalPrivate(void) const {
        return ZFBitTest(_stateFlags, _stateFlags_isInternalPrivate);
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

public:
    static ZFProperty *_ZFP_ZFPropertyRegister(
            ZF_IN zfbool isUserRegister
            , ZF_IN zfbool isDynamicRegister
            , ZF_IN ZFObject *dynamicRegisterUserData
            , ZF_IN const ZFClass *ownerClass
            , ZF_IN const zfstring &name
            , ZF_IN const zfstring &typeName
            , ZF_IN const zfstring &typeIdName
            , ZF_IN const ZFMethod *setterMethod
            , ZF_IN const ZFMethod *getterMethod
            , ZF_IN const ZFClass *propertyClassOfRetainProperty
            , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
            , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
            , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
            , ZF_IN ZFPropertyCallbackUserRegisterInitValueSetup callbackUserRegisterInitValueSetup
            , ZF_IN ZFPropertyCallbackDynamicRegisterInitValueGetter callbackDynamicRegisterInitValueGetter
            , ZF_IN _ZFP_ZFPropertyCallbackEnsureInit callbackEnsureInit
            , ZF_IN _ZFP_ZFPropertyCallbackDealloc callbackDealloc
            );
    static void _ZFP_ZFPropertyUnregister(ZF_IN const ZFProperty *propertyInfo);

    void _ZFP_ZFPropertyInit(
            ZF_IN zfbool isUserRegister
            , ZF_IN zfbool isDynamicRegister
            , ZF_IN ZFObject *dynamicRegisterUserData
            , ZF_IN const ZFClass *ownerClass
            , ZF_IN const zfstring &name
            , ZF_IN const zfstring &typeName
            , ZF_IN const zfstring &typeIdName
            , ZF_IN const ZFMethod *setterMethod
            , ZF_IN const ZFMethod *getterMethod
            , ZF_IN const ZFClass *propertyClassOfRetainProperty
            );
    ZFProperty *_ZFP_ZFProperty_removeConst(void) const {
        return const_cast<ZFProperty *>(this);
    }
    inline ZFObject *_ZFP_ZFProperty_dynamicRegisterUserDataWrapper(void) const {
        return _dynamicRegisterUserDataWrapper;
    }
    void _ZFP_ZFProperty_dynamicRegisterUserDataWrapper(ZF_IN ZFObject *v) const;

    _ZFP_ZFPropertyCallbackEnsureInit _ZFP_ZFProperty_callbackEnsureInit(void) const {
        return _callbackEnsureInit;
    }
    _ZFP_ZFPropertyCallbackDealloc _ZFP_ZFProperty_callbackDealloc(void) const {
        return _callbackDealloc;
    }

private:
    enum {
        _stateFlags_isInternal = 1 << 0,
        _stateFlags_isInternalPrivate = 1 << 1,
        _stateFlags_isUserRegister = 1 << 2,
        _stateFlags_isDynamicRegister = 1 << 3,
    };
private:
    zfuint _refCount;
    zfuint _stateFlags;
    ZFSigName _propertyId;
    ZFObject *_dynamicRegisterUserData;
    ZFObject *_dynamicRegisterUserDataWrapper;
    const ZFClass *_ownerClass;
    ZFSigName _propertyName;
    ZFSigName _propertyTypeName;
    ZFSigName _propertyTypeId;
    const ZFMethod *_setterMethod;
    const ZFMethod *_getterMethod;
    const ZFClass *_propertyClassOfRetainProperty;
    _ZFP_ZFPropertyCallbackEnsureInit _callbackEnsureInit;
    _ZFP_ZFPropertyCallbackDealloc _callbackDealloc;
public:
    ZFCoreArray<_ZFP_PropLifeCycleData> _ZFP_ZFPropertyLifeCycle_OnInit; // ordered from parent to child
    ZFCoreArray<_ZFP_PropLifeCycleData> _ZFP_ZFPropertyLifeCycle_OnUpdate;
    ZFCoreArray<_ZFP_PropLifeCycleData> _ZFP_ZFPropertyLifeCycle_OnAttach;
    ZFCoreArray<_ZFP_PropLifeCycleData> _ZFP_ZFPropertyLifeCycle_OnDetach;
};

// ============================================================
/** @brief see #ZFPropertyGetAll */
extern ZFLIB_ZFCore void ZFPropertyGetAllT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret);
/**
 * @brief get all property currently registered, for debug use only
 */
inline ZFCoreArray<const ZFProperty *> ZFPropertyGetAll(void) {
    ZFCoreArray<const ZFProperty *> ret;
    ZFPropertyGetAllT(ret);
    return ret;
}

// ============================================================
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFPropertyRegisterHolder {
public:
    _ZFP_ZFPropertyRegisterHolder(
            ZF_IN zfbool isUserRegister
            , ZF_IN zfbool isDynamicRegister
            , ZF_IN ZFObject *dynamicRegisterUserData
            , ZF_IN const ZFClass *ownerClass
            , ZF_IN const zfstring &name
            , ZF_IN const zfstring &typeName
            , ZF_IN const zfstring &typeIdName
            , ZF_IN const ZFMethod *setterMethod
            , ZF_IN const ZFMethod *getterMethod
            , ZF_IN const ZFClass *propertyClassOfRetainProperty
            , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
            , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
            , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
            , ZF_IN ZFPropertyCallbackUserRegisterInitValueSetup callbackUserRegisterInitValueSetup
            , ZF_IN ZFPropertyCallbackDynamicRegisterInitValueGetter callbackDynamicRegisterInitValueGetter
            , ZF_IN _ZFP_ZFPropertyCallbackEnsureInit callbackEnsureInit
            , ZF_IN _ZFP_ZFPropertyCallbackDealloc callbackDealloc
            )
    : propertyInfo(ZFProperty::_ZFP_ZFPropertyRegister(
        isUserRegister
        , isDynamicRegister
        , dynamicRegisterUserData
        , ownerClass
        , name
        , typeName
        , typeIdName
        , setterMethod
        , getterMethod
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
        ZFProperty::_ZFP_ZFPropertyUnregister(this->propertyInfo);
    }
public:
    ZFProperty *propertyInfo;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProperty_h_

