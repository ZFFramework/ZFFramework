#include "ZFPropertyDynamicRegister.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFPropertyDynamicRegisterParamPrivate {
public:
    zfuint refCount;
    zfbool zfunsafe_disableChecker;
    zfauto propertyDynamicRegisterUserData;
    const ZFClass *propertyOwnerClass;
    zfstring propertyTypeId;
    zfstring propertyTypeName;
    zfstring propertyName;
    const ZFClass *propertyClassOfRetainProperty;
    ZFPropertyCallbackDynamicRegisterInitValueGetter propertyInitValueCallback;
    ZFMethodPrivilegeType propertySetterType;
    ZFMethodPrivilegeType propertyGetterType;

    const ZFMethod *propertyCustomImplSetterMethod;
    const ZFMethod *propertyCustomImplGetterMethod;
    ZFPropertyCallbackIsValueAccessed propertyCustomImplCallbackIsValueAccessed;
    ZFPropertyCallbackIsInitValue propertyCustomImplCallbackIsInitValue;
    ZFPropertyCallbackValueReset propertyCustomImplCallbackValueReset;

public:
    _ZFP_ZFPropertyDynamicRegisterParamPrivate(void)
    : refCount(1)
    , zfunsafe_disableChecker(zffalse)
    , propertyDynamicRegisterUserData(zfnull)
    , propertyOwnerClass()
    , propertyTypeId()
    , propertyTypeName()
    , propertyName()
    , propertyClassOfRetainProperty(zfnull)
    , propertyInitValueCallback(zfnull)
    , propertySetterType(ZFMethodPrivilegeTypePublic)
    , propertyGetterType(ZFMethodPrivilegeTypePublic)

    , propertyCustomImplSetterMethod(zfnull)
    , propertyCustomImplGetterMethod(zfnull)
    , propertyCustomImplCallbackIsValueAccessed(zfnull)
    , propertyCustomImplCallbackIsInitValue(zfnull)
    , propertyCustomImplCallbackValueReset(zfnull)
    {
    }
};

// ============================================================
ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyDynamicRegisterUserData(ZF_IN ZFObject *propertyDynamicRegisterUserData) {
    d->propertyDynamicRegisterUserData = propertyDynamicRegisterUserData;
    return *this;
}
zfany ZFPropertyDynamicRegisterParam::propertyDynamicRegisterUserData(void) const {
    return d->propertyDynamicRegisterUserData;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyOwnerClass(ZF_IN const ZFClass *propertyOwnerClass) {
    d->propertyOwnerClass = propertyOwnerClass;
    return *this;
}
const ZFClass *ZFPropertyDynamicRegisterParam::propertyOwnerClass(void) const {
    return d->propertyOwnerClass;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyTypeId(ZF_IN const zfstring &propertyTypeId) {
    d->propertyTypeId = propertyTypeId;
    return *this;
}
const zfstring &ZFPropertyDynamicRegisterParam::propertyTypeId(void) const {
    return d->propertyTypeId;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyTypeName(ZF_IN const zfstring &propertyTypeName) {
    d->propertyTypeName = propertyTypeName;
    return *this;
}
const zfstring &ZFPropertyDynamicRegisterParam::propertyTypeName(void) const {
    return (d->propertyTypeName ? d->propertyTypeName : d->propertyTypeId);
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyName(ZF_IN const zfstring &propertyName) {
    d->propertyName = propertyName;
    return *this;
}
const zfstring &ZFPropertyDynamicRegisterParam::propertyName(void) const {
    return d->propertyName;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyClassOfRetainProperty(ZF_IN const ZFClass *propertyClassOfRetainProperty) {
    d->propertyClassOfRetainProperty = propertyClassOfRetainProperty;
    return *this;
}
const ZFClass *ZFPropertyDynamicRegisterParam::propertyClassOfRetainProperty(void) const {
    return d->propertyClassOfRetainProperty;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyInitValueCallback(ZF_IN ZFPropertyCallbackDynamicRegisterInitValueGetter propertyInitValueCallback) {
    d->propertyInitValueCallback = propertyInitValueCallback;
    return *this;
}
ZFPropertyCallbackDynamicRegisterInitValueGetter ZFPropertyDynamicRegisterParam::propertyInitValueCallback(void) const {
    return d->propertyInitValueCallback;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertySetterType(ZF_IN ZFMethodPrivilegeType propertySetterType) {
    d->propertySetterType = propertySetterType;
    return *this;
}
ZFMethodPrivilegeType ZFPropertyDynamicRegisterParam::propertySetterType(void) const {
    return d->propertySetterType;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyGetterType(ZF_IN ZFMethodPrivilegeType propertyGetterType) {
    d->propertyGetterType = propertyGetterType;
    return *this;
}
ZFMethodPrivilegeType ZFPropertyDynamicRegisterParam::propertyGetterType(void) const {
    return d->propertyGetterType;
}

// ============================================================
ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyCustomImpl(
        ZF_IN const ZFMethod *propertySetterMethod
        , ZF_IN const ZFMethod *propertyGetterMethod
        , ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed
        , ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue
        , ZF_IN ZFPropertyCallbackValueReset callbackValueReset
        ) {
    d->propertyCustomImplSetterMethod = propertySetterMethod;
    d->propertyCustomImplGetterMethod = propertyGetterMethod;
    d->propertyCustomImplCallbackIsValueAccessed = callbackIsValueAccessed;
    d->propertyCustomImplCallbackIsInitValue = callbackIsInitValue;
    d->propertyCustomImplCallbackValueReset = callbackValueReset;
    return *this;
}

const ZFMethod *ZFPropertyDynamicRegisterParam::propertyCustomImplSetterMethod(void) const {
    return d->propertyCustomImplSetterMethod;
}
const ZFMethod *ZFPropertyDynamicRegisterParam::propertyCustomImplGetterMethod(void) const {
    return d->propertyCustomImplGetterMethod;
}
ZFPropertyCallbackIsValueAccessed ZFPropertyDynamicRegisterParam::propertyCustomImplCallbackIsValueAccessed(void) const {
    return d->propertyCustomImplCallbackIsValueAccessed;
}
ZFPropertyCallbackIsInitValue ZFPropertyDynamicRegisterParam::propertyCustomImplCallbackIsInitValue(void) const {
    return d->propertyCustomImplCallbackIsInitValue;
}
ZFPropertyCallbackValueReset ZFPropertyDynamicRegisterParam::propertyCustomImplCallbackValueReset(void) const {
    return d->propertyCustomImplCallbackValueReset;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::zfunsafe_disableChecker(ZF_IN zfbool disableChecker) {
    d->zfunsafe_disableChecker = disableChecker;
    return *this;
}
zfbool ZFPropertyDynamicRegisterParam::zfunsafe_disableChecker(void) const {
    return d->zfunsafe_disableChecker;
}

// ============================================================
ZFPropertyDynamicRegisterParam::ZFPropertyDynamicRegisterParam(void)
: d(zfnew(_ZFP_ZFPropertyDynamicRegisterParamPrivate))
{
}
ZFPropertyDynamicRegisterParam::ZFPropertyDynamicRegisterParam(ZF_IN ZFPropertyDynamicRegisterParam const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFPropertyDynamicRegisterParam::~ZFPropertyDynamicRegisterParam(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
}
ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::operator = (ZF_IN ZFPropertyDynamicRegisterParam const &ref) {
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFPropertyDynamicRegisterParam::operator == (ZF_IN ZFPropertyDynamicRegisterParam const &ref) const {
    return (d == ref.d);
}

void ZFPropertyDynamicRegisterParam::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += "<ZFPropertyDynamicRegisterParam ";

    if(this->propertyClassOfRetainProperty() != zfnull) {
        ret += this->propertyClassOfRetainProperty()->classNameFull();
        ret += " ";
    }
    else if(!zfstringIsEmpty(this->propertyTypeName())) {
        ret += this->propertyTypeName();
        ret += " ";
    }
    else if(!zfstringIsEmpty(this->propertyTypeId())) {
        ret += this->propertyTypeId();
        ret += " ";
    }

    if(this->propertyOwnerClass() != zfnull) {
        ret += this->propertyOwnerClass()->classNameFull();
        ret += "::";
    }

    ret += this->propertyName();

    ret += ">";
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyDynamicRegister
        , ZFMP_IN(const ZFPropertyDynamicRegisterParam &, param)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFPropertyDynamicUnregister
        , ZFMP_IN(const ZFProperty *, property)
        )

ZFOBJECT_REGISTER(ZFPropertyInvokeData)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFPropertyInvokeData, zfany, invokerObject)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFPropertyInvokeData, const ZFProperty *, invokerProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFPropertyInvokeData, zfauto, propertyValue)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFPropertyInvokeData, zfauto, propertyValueOld)

ZF_NAMESPACE_GLOBAL_END
#endif

